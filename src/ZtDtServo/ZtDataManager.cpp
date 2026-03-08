/*!
 * \file ZtDataManager.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ZtDataManager.h"
#include "ZtDtRunner.h"
#include "ZtHelper.h"

#include "../Includes/ZTSDataDef.hpp"
#include "../Includes/ZTSVariant.hpp"
#include "../Includes/ZTSContractInfo.hpp"

#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/DLLHelper.hpp"

#include "../ZTSTools/ZTSLogger.h"
#include "../ZTSTools/ZTSDataFactory.h"


ZTSDataFactory g_dataFact;

ZtDataManager::ZtDataManager()
	: _bd_mgr(NULL)
	, _hot_mgr(NULL)
	, _runner(NULL)
	, _reader(NULL)
	, _rt_bars(NULL)
{
}


ZtDataManager::~ZtDataManager()
{
	for(auto& m : _bars_cache)
	{
		if (m.second._bars != NULL)
			m.second._bars->release();
	}
	_bars_cache.clear();
}

bool ZtDataManager::initStore(ZTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	std::string module = cfg->getCString("module");
	if (module.empty())
		module = "ZtDataStorage";

	module = ZtHelper::get_module_dir() + DLLHelper::wrap_module(module.c_str());
	DllHandle libParser = DLLHelper::load_library(module.c_str());
	if (libParser)
	{
		FuncCreateRdmDtReader pFuncCreateReader = (FuncCreateRdmDtReader)DLLHelper::get_symbol(libParser, "createRdmDtReader");
		if (pFuncCreateReader == NULL)
		{
			ZTSLogger::error("Initializing of random data reader failed: function createRdmDtReader not found...");
		}

		FuncDeleteRdmDtReader pFuncDeleteReader = (FuncDeleteRdmDtReader)DLLHelper::get_symbol(libParser, "deleteRdmDtReader");
		if (pFuncDeleteReader == NULL)
		{
			ZTSLogger::error("Initializing of random data reader failed: function deleteRdmDtReader not found...");
		}

		if (pFuncCreateReader && pFuncDeleteReader)
		{
			_reader = pFuncCreateReader();
			_remover = pFuncDeleteReader;
		}

	}
	else
	{
		ZTSLogger::error("Initializing of random data reader failed: loading module {} failed...", module);

	}

	_reader->init(cfg, this);
	return true;
}

bool ZtDataManager::init(ZTSVariant* cfg, ZtDtRunner* runner)
{
	_runner = runner;
	if (_runner)
	{
		_bd_mgr = &_runner->getBaseDataMgr();
		_hot_mgr = &_runner->getHotMgr();
	}

	_align_by_section = cfg->getBoolean("align_by_section");

	ZTSLogger::info("Resampled bars will be aligned by section: {}", _align_by_section ? "yes" : " no");

	return initStore(cfg->get("store"));
}

void ZtDataManager::reader_log(ZTSLogLevel ll, const char* message)
{
	ZTSLogger::log_raw(ll, message);
}

ZTSTickSlice* ZtDataManager::get_tick_slices_by_range(const char* stdCode,uint64_t stime, uint64_t etime /* = 0 */)
{
	stime = stime * 100000;
	etime = etime * 100000;
	return _reader->readTickSliceByRange(stdCode, stime, etime);
}

ZTSTickSlice* ZtDataManager::get_tick_slice_by_date(const char* stdCode, uint32_t uDate /* = 0 */)
{
	return _reader->readTickSliceByDate(stdCode, uDate);
}

ZTSOrdQueSlice* ZtDataManager::get_order_queue_slice(const char* stdCode,uint64_t stime, uint64_t etime /* = 0 */)
{
	stime = stime * 100000;
	etime = etime * 100000;
	return _reader->readOrdQueSliceByRange(stdCode, stime, etime);
}

ZTSOrdDtlSlice* ZtDataManager::get_order_detail_slice(const char* stdCode,uint64_t stime, uint64_t etime /* = 0 */)
{
	stime = stime * 100000;
	etime = etime * 100000;
	return _reader->readOrdDtlSliceByRange(stdCode, stime, etime);
}

ZTSTransSlice* ZtDataManager::get_transaction_slice(const char* stdCode,uint64_t stime, uint64_t etime /* = 0 */)
{
	stime = stime * 100000;
	etime = etime * 100000;
	return _reader->readTransSliceByRange(stdCode, stime, etime);
}

ZTSSessionInfo* ZtDataManager::get_session_info(const char* sid, bool isCode /* = false */)
{
	if (!isCode)
		return _bd_mgr->getSession(sid);

	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(sid, _hot_mgr);
	ZTSCommodityInfo* cInfo = _bd_mgr->getCommodity(codeInfo._exchg, codeInfo._product);
	if (cInfo == NULL)
		return NULL;

	return cInfo->getSessionInfo();
}

ZTSKlineSlice* ZtDataManager::get_skline_slice_by_date(const char* stdCode, uint32_t secs, uint32_t uDate /* = 0 */)
{
	std::string key = StrUtil::printf("%s-%u-s%u", stdCode, uDate, secs);

	//只有非基础周期的会进到下面的步骤
	ZTSSessionInfo* sInfo = get_session_info(stdCode, true);
	BarCache& barCache = _bars_cache[key];
	barCache._period = KP_Tick;
	barCache._times = secs;
	if (barCache._bars == NULL)
	{
		//第一次将全部数据缓存到内存中
		ZTSTickSlice* ticks = _reader->readTickSliceByDate(stdCode, uDate);
		if (ticks != NULL)
		{
			ZTSKlineData* kData = g_dataFact.extractKlineData(ticks, secs, sInfo, true);
			barCache._bars = kData;		
			ticks->release();
		}
		else
		{
			return NULL;
		}
	}
	
	if (barCache._bars == NULL)
		return NULL;

	ZTSBarStruct* rtHead = barCache._bars->at(0);
	ZTSKlineSlice* slice = ZTSKlineSlice::create(stdCode, KP_Tick, secs, rtHead, barCache._bars->size());
	return slice;
}

ZTSKlineSlice* ZtDataManager::get_kline_slice_by_date(const char* stdCode, ZTSKlinePeriod period, uint32_t times, uint32_t uDate /* = 0 */)
{
	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	uint64_t stime = _bd_mgr->getBoundaryTime(codeInfo.stdCommID(), uDate, false, true);
	uint64_t etime = _bd_mgr->getBoundaryTime(codeInfo.stdCommID(), uDate, false, false);
	return get_kline_slice_by_range(stdCode, period, times, stime, etime);
}

ZTSKlineSlice* ZtDataManager::get_kline_slice_by_range(const char* stdCode, ZTSKlinePeriod period, uint32_t times,uint64_t stime, uint64_t etime /* = 0 */)
{
	if (times == 1)
	{
		return _reader->readKlineSliceByRange(stdCode, period, stime, etime);
	}

	//只有非基础周期的会进到下面的步骤
	ZTSSessionInfo* sInfo = get_session_info(stdCode, true);
	std::string key = StrUtil::printf("%s-%u-%u", stdCode, period, times);
	BarCache& barCache = _bars_cache[key];
	barCache._period = period;
	barCache._times = times;
	if(barCache._bars == NULL)
	{
		//第一次将全部数据缓存到内存中
		ZTSKlineSlice* rawData = _reader->readKlineSliceByCount(stdCode, period, UINT_MAX, 0);
		if (rawData != NULL)
		{
			ZTSKlineData* kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, false);
			barCache._bars = kData;

			//不管如何，都删除最后一条K线
			//不能通过闭合标记判断，因为读取的基础周期可能本身没有闭合
			if (barCache._bars->size() > 0)
			{
				auto& bars = barCache._bars->getDataRef();
				bars.erase(bars.begin() + bars.size() - 1, bars.end());
			}

			if (period == KP_DAY)
				barCache._last_bartime = kData->date(-1);
			else
			{
				uint64_t lasttime = kData->time(-1);
				barCache._last_bartime = 199000000000 + lasttime;
			}

			rawData->release();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		//后面则增量更新
		ZTSKlineSlice* rawData = _reader->readKlineSliceByRange(stdCode, period, barCache._last_bartime, 0);
		if (rawData != NULL)
		{
			for(int32_t idx = 0; idx < rawData->size(); idx ++)
			{
				uint64_t barTime = 0;
				if (period == KP_DAY)
					barTime = rawData->at(0)->date;
				else
					barTime = 199000000000 + rawData->at(0)->time;
				
				//只有时间上次记录的最后一条时间，才可以用于更新K线
				if(barTime <= barCache._last_bartime)
					continue;

				g_dataFact.updateKlineData(barCache._bars, rawData->at(idx), sInfo, _align_by_section);
			}

			//不管如何，都删除最后一条K线
			//不能通过闭合标记判断，因为读取的基础周期可能本身没有闭合
			if(barCache._bars->size() > 0)
			{
				auto& bars = barCache._bars->getDataRef();
				bars.erase(bars.begin() + bars.size() - 1, bars.end());
			}

			if (period == KP_DAY)
				barCache._last_bartime = barCache._bars->date(-1);
			else
			{
				uint64_t lasttime = barCache._bars->time(-1);
				barCache._last_bartime = 199000000000 + lasttime;
			}
			

			rawData->release();
		}
	}

	//最后到缓存中定位
	bool isDay = period == KP_DAY;
	uint32_t rDate, rTime, lDate, lTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);
	lDate = (uint32_t)(stime / 10000);
	lTime = (uint32_t)(stime % 10000);

	ZTSBarStruct eBar;
	eBar.date = rDate;
	eBar.time = (rDate - 19900000) * 10000 + rTime;

	ZTSBarStruct sBar;
	sBar.date = lDate;
	sBar.time = (lDate - 19900000) * 10000 + lTime;

	uint32_t eIdx, sIdx;
	auto& bars = barCache._bars->getDataRef();
	auto eit = std::lower_bound(bars.begin(), bars.end(), eBar, [isDay](const ZTSBarStruct& a, const ZTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});


	if (eit == bars.end())
		eIdx = bars.size() - 1;
	else
	{
		if ((isDay && eit->date > eBar.date) || (!isDay && eit->time > eBar.time))
		{
			eit--;
		}

		eIdx = eit - bars.begin();
	}

	auto sit = std::lower_bound(bars.begin(), eit, sBar, [isDay](const ZTSBarStruct& a, const ZTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});
	sIdx = sit - bars.begin();
	uint32_t rtCnt = eIdx - sIdx + 1;
	ZTSBarStruct* rtHead = barCache._bars->at(sIdx);
	ZTSKlineSlice* slice = ZTSKlineSlice::create(stdCode, period, times, rtHead, rtCnt);
	return slice;
}

ZTSKlineSlice* ZtDataManager::get_kline_slice_by_count(const char* stdCode, ZTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime /* = 0 */)
{
	if (times == 1)
	{
		return _reader->readKlineSliceByCount(stdCode, period, count, etime);
	}

	//只有非基础周期的会进到下面的步骤
	ZTSSessionInfo* sInfo = get_session_info(stdCode, true);
	std::string key = StrUtil::printf("%s-%u-%u", stdCode, period, times);
	BarCache& barCache = _bars_cache[key];
	barCache._period = period;
	barCache._times = times;

	const char* tag = PERIOD_NAME[period-KP_Tick];

	if (barCache._bars == NULL)
	{
		//第一次将全部数据缓存到内存中
		ZTSLogger::info("Caching all {} bars of {}...", tag, stdCode);
		ZTSKlineSlice* rawData = _reader->readKlineSliceByCount(stdCode, period, UINT_MAX, 0);
		if (rawData != NULL)
		{
			ZTSLogger::info("Resampling {} {} bars by {}-TO-1 of {}...", rawData->size(), tag, times, stdCode);
			ZTSKlineData* kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, true);
			barCache._bars = kData;

			//如果不是日线，要考虑最后一条K线是否闭合的情况
			//这里采用保守的方案，如果本地时间大于最后一条K线的时间，则认为真正闭合了
			if (period != KP_DAY)
			{
				uint64_t last_bartime = 0;
				last_bartime = 199000000000 + kData->time(-1);

				uint64_t now = TimeUtils::getYYYYMMDDhhmmss() / 100;
				if (now <= last_bartime && barCache._bars->size() > 0)
				{
					auto& bars = barCache._bars->getDataRef();
					bars.erase(bars.begin() + bars.size() - 1, bars.end());
				}
			}


			if (period == KP_DAY)
				barCache._last_bartime = kData->date(-1);
			else
			{
				uint64_t lasttime = kData->time(-1);
				barCache._last_bartime = 199000000000 + lasttime;
			}

			rawData->release();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		//后面则增量更新
		ZTSKlineSlice* rawData = _reader->readKlineSliceByRange(stdCode, period, barCache._last_bartime, 0);
		if (rawData != NULL)
		{
			ZTSLogger::info("{} {} bars of {} updated, adding to cache...", rawData->size(), tag, stdCode);
			for (int32_t idx = 0; idx < rawData->size(); idx++)
			{
				uint64_t barTime = 0;
				if (period == KP_DAY)
					barTime = rawData->at(0)->date;
				else
					barTime = 199000000000 + rawData->at(0)->time;

				//只有时间上次记录的最后一条时间，才可以用于更新K线
				if (barTime <= barCache._last_bartime)
					continue;

				g_dataFact.updateKlineData(barCache._bars, rawData->at(idx), sInfo, _align_by_section);
			}

			//如果不是日线，要考虑最后一条K线是否闭合的情况
			//这里采用保守的方案，如果本地时间大于最后一条K线的时间，则认为真正闭合了
			if (period != KP_DAY)
			{
				uint64_t last_bartime = 0;
				last_bartime = 199000000000 + barCache._bars->time(-1);

				uint64_t now = TimeUtils::getYYYYMMDDhhmmss() / 100;
				if (now <= last_bartime && barCache._bars->size() > 0)
				{
					auto& bars = barCache._bars->getDataRef();
					bars.erase(bars.begin() + bars.size() - 1, bars.end());
				}
			}

			if (period == KP_DAY)
				barCache._last_bartime = barCache._bars->date(-1);
			else
			{
				uint64_t lasttime = barCache._bars->time(-1);
				barCache._last_bartime = 199000000000 + lasttime;
			}


			rawData->release();
		}
	}

	//最后到缓存中定位
	bool isDay = period == KP_DAY;
	uint32_t rDate, rTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);

	ZTSBarStruct eBar;
	eBar.date = rDate;
	eBar.time = (rDate - 19900000) * 10000 + rTime;

	uint32_t eIdx, sIdx;
	auto& bars = barCache._bars->getDataRef();
	auto eit = std::lower_bound(bars.begin(), bars.end(), eBar, [isDay](const ZTSBarStruct& a, const ZTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});


	if (eit == bars.end())
		eIdx = bars.size() - 1;
	else
	{
		if ((isDay && eit->date > eBar.date) || (!isDay && eit->time > eBar.time))
		{
			eit--;
		}

		eIdx = eit - bars.begin();
	}

	sIdx = (eIdx + 1 >= count) ? (eIdx + 1 - count) : 0;
	uint32_t rtCnt = eIdx - sIdx + 1;
	ZTSBarStruct* rtHead = barCache._bars->at(sIdx);
	ZTSKlineSlice* slice = ZTSKlineSlice::create(stdCode, period, times, rtHead, rtCnt);
	return slice;
}

ZTSTickSlice* ZtDataManager::get_tick_slice_by_count(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	etime = etime * 100000;
	return _reader->readTickSliceByCount(stdCode, count, etime);
}

double ZtDataManager::get_exright_factor(const char* stdCode, ZTSCommodityInfo* commInfo /* = NULL */)
{
	if (commInfo == NULL)
		return 1.0;

	if (commInfo->isStock())
		return _reader->getAdjFactorByDate(stdCode, 0);
	else
	{
		const char* ruleTag = _hot_mgr->getRuleTag(stdCode);
		if (strlen(ruleTag) > 0)
			return _hot_mgr->getRuleFactor(ruleTag, commInfo->getFullPid(), 0);
	}

	return 1.0;
}

void ZtDataManager::subscribe_bar(const char* stdCode, ZTSKlinePeriod period, uint32_t times)
{
	std::string key = fmtutil::format("{}-{}-{}", stdCode, (uint32_t)period, times);

	uint32_t curDate = TimeUtils::getCurDate();
	uint64_t etime = (uint64_t)curDate * 10000 + 2359;

	if (times == 1)
	{
		ZTSKlineSlice* slice = _reader->readKlineSliceByCount(stdCode, period, 10, etime);
		if (slice == NULL)
			return;

		ZTSKlineData* kline = ZTSKlineData::create(stdCode, slice->size());
		kline->setPeriod(period);
		uint32_t offset = 0;
		for(uint32_t blkIdx = 0; blkIdx < slice->get_block_counts(); blkIdx++)
		{
			memcpy(kline->getDataRef().data() + offset, slice->get_block_addr(blkIdx), sizeof(ZTSBarStruct)*slice->get_block_size(blkIdx));
			offset += slice->get_block_size(blkIdx);
		}
		
		{
			StdUniqueLock lock(_mtx_rtbars);
			if (_rt_bars == NULL)
				_rt_bars = RtBarMap::create();

			_rt_bars->add(key, kline, false);
		}

		slice->release();
	}
	else
	{
		//只有非基础周期的会进到下面的步骤
		ZTSSessionInfo* sInfo = get_session_info(stdCode, true);
		ZTSKlineSlice* rawData = _reader->readKlineSliceByCount(stdCode, period, 10*times, 0);
		if (rawData != NULL)
		{
			ZTSKlineData* kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, true);
			{
				StdUniqueLock lock(_mtx_rtbars);
				if (_rt_bars == NULL)
					_rt_bars = RtBarMap::create();
				_rt_bars->add(key, kData, false);
			}
			rawData->release();
		}
	}

	ZTSLogger::info("Realtime bar {} has subscribed", key);
}

void ZtDataManager::clear_subbed_bars()
{
	StdUniqueLock lock(_mtx_rtbars);
	if (_rt_bars)
		_rt_bars->clear();
}

void ZtDataManager::update_bars(const char* stdCode, ZTSTickData* newTick)
{
	if (_rt_bars == NULL)
		return;

	StdUniqueLock lock(_mtx_rtbars);
	auto it = _rt_bars->begin();
	for(; it != _rt_bars->end(); it++)
	{
		ZTSKlineData* kData = (ZTSKlineData*)it->second;
		if (strcmp(kData->code(), stdCode) != 0)
			continue;

		ZTSSessionInfo* sInfo = NULL;
		if (newTick->getContractInfo())
			sInfo = newTick->getContractInfo()->getCommInfo()->getSessionInfo();
		else
			sInfo = get_session_info(kData->code(), true);
		g_dataFact.updateKlineData(kData, newTick, sInfo, _align_by_section);
		ZTSBarStruct* lastBar = kData->at(-1);

		std::string speriod;
		uint32_t times = kData->times();
		switch (kData->period())
		{
		case KP_Minute1:
			speriod = fmtutil::format("m{}", times);
			break;
		case KP_Minute5:
			speriod = fmtutil::format("m{}", times*5);
			break;
		default:
			speriod = fmtutil::format("d{}", times);
			break;
		}

		_runner->trigger_bar(stdCode, speriod.c_str(), lastBar);
	}
}

void ZtDataManager::clear_cache()
{
	if (_reader == NULL)
	{
		ZTSLogger::warn("DataReader not initialized, clearing canceled");
		return;
	}

	_reader->clearCache();
	ZTSLogger::warn("All cache cleared");
}