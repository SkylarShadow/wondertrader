/*!
 * \file ZtSimpDataMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#include "ZtSimpDataMgr.h"
#include "ZtExecRunner.h"
#include "../ZtCore/ZtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Includes/ZTSDataDef.hpp"
#include "../Includes/ZTSVariant.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Includes/ZTSSessionInfo.hpp"

#include "../ZTSTools/ZTSLogger.h"
#include "../ZTSTools/ZTSDataFactory.h"

USING_NS_ZTP;


ZTSDataFactory g_dataFact;

ZtSimpDataMgr::ZtSimpDataMgr()
	: _reader(NULL)
	, _runner(NULL)
	, _bars_cache(NULL)
	, _rt_tick_map(NULL)
{
}


ZtSimpDataMgr::~ZtSimpDataMgr()
{
	if (_rt_tick_map)
		_rt_tick_map->release();
}

bool ZtSimpDataMgr::initStore(ZTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	std::string module = cfg->getCString("module");
	if (module.empty())
		module = ZtHelper::getInstDir() + DLLHelper::wrap_module("ZtDataStorage");
	else
		module = ZtHelper::getInstDir() + DLLHelper::wrap_module(module.c_str());

	DllHandle hInst = DLLHelper::load_library(module.c_str());
	if (hInst == NULL)
	{
		ZTSLogger::error("Data reader {} loading failed", module.c_str());
		return false;
	}

	FuncCreateDataReader funcCreator = (FuncCreateDataReader)DLLHelper::get_symbol(hInst, "createDataReader");
	if (funcCreator == NULL)
	{
		ZTSLogger::error("Data reader {} loading failed: entrance function createDataReader not found", module.c_str());
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader = funcCreator();
	if (_reader == NULL)
	{
		ZTSLogger::error("Data reader {} creating api failed", module.c_str());
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader->init(cfg, this);

	_s_info = _runner->get_session_info(cfg->getCString("session"), false);

	return true;
}

bool ZtSimpDataMgr::init(ZTSVariant* cfg, ZtExecRunner* runner)
{
	_runner = runner;
	return initStore(cfg->get("store"));
}

void ZtSimpDataMgr::on_all_bar_updated(uint32_t updateTime)
{

}

IBaseDataMgr* ZtSimpDataMgr::get_basedata_mgr()
{
	return _runner->get_bd_mgr();
}

IHotMgr* ZtSimpDataMgr::get_hot_mgr()
{
	return _runner->get_hot_mgr();
}

uint32_t ZtSimpDataMgr::get_date()
{
	return _cur_date;
}

uint32_t ZtSimpDataMgr::get_min_time()
{
	return _cur_min_time;
}

uint32_t ZtSimpDataMgr::get_secs()
{
	return _cur_secs;
}

void ZtSimpDataMgr::reader_log(ZTSLogLevel ll, const char* message)
{
	ZTSLogger::log_raw(ll, message);
}

void ZtSimpDataMgr::on_bar(const char* code, ZTSKlinePeriod period, ZTSBarStruct* newBar)
{

}

void ZtSimpDataMgr::handle_push_quote(const char* stdCode, ZTSTickData* curTick)
{
	if (curTick == NULL)
		return;

	if (_rt_tick_map == NULL)
		_rt_tick_map = DataCacheMap::create();

	_rt_tick_map->add(stdCode, curTick, true);

	uint32_t uDate = curTick->actiondate();
	uint32_t uTime = curTick->actiontime();

	if (_cur_date != 0 && (uDate < _cur_date || (uDate == _cur_date && uTime < _cur_act_time)))
	{
		return;
	}

	_cur_date = uDate;
	_cur_act_time = uTime;

	uint32_t _cur_raw_time = _cur_act_time / 100000;
	uint32_t _cur_secs = _cur_act_time % 100000;
	uint32_t minutes = _s_info->timeToMinutes(_cur_raw_time);
	bool isSecEnd = _s_info->isLastOfSection(_cur_raw_time);
	if (isSecEnd)
	{
		minutes--;
	}
	minutes++;
	_cur_min_time = _s_info->minuteToTime(minutes);
	_cur_tdate = curTick->tradingdate();
}

ZTSTickData* ZtSimpDataMgr::grab_last_tick(const char* code)
{
	if (_rt_tick_map == NULL)
		return NULL;

	ZTSTickData* curTick = (ZTSTickData*)_rt_tick_map->get(code);
	if (curTick == NULL)
		return NULL;

	curTick->retain();
	return curTick;
}


ZTSTickSlice* ZtSimpDataMgr::get_tick_slice(const char* code, uint32_t count, uint64_t etime /*= 0*/)
{
	if (_reader == NULL)
		return NULL;

	return _reader->readTickSlice(code, count, etime);
}


ZTSKlineSlice* ZtSimpDataMgr::get_kline_slice(const char* stdCode, ZTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime /*= 0*/)
{
	if (_reader == NULL)
		return NULL;

	std::string key = StrUtil::printf("%s-%u", stdCode, period);

	if (times == 1)
	{
		return _reader->readKlineSlice(stdCode, period, count, etime);
	}

	//只有非基础周期的会进到下面的步骤
	ZTSSessionInfo* sInfo = _runner->get_session_info(stdCode, true);

	if (_bars_cache == NULL)
		_bars_cache = DataCacheMap::create();

	key = StrUtil::printf("%s-%u-%u", stdCode, period, times);

	ZTSKlineData* kData = (ZTSKlineData*)_bars_cache->get(key);
	//如果缓存里的K线条数大于请求的条数, 则直接返回
	if (kData == NULL || kData->size() < count)
	{
		uint32_t realCount = count * times + times;
		ZTSKlineSlice* rawData = _reader->readKlineSlice(stdCode, period, realCount, etime);
		if (rawData != NULL)
		{
			kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, true);
			rawData->release();
		}
		else
		{
			return NULL;
		}

		if (kData)
			_bars_cache->add(key, kData, false);
	}

	int32_t sIdx = 0;
	uint32_t rtCnt = min(kData->size(), count);
	sIdx = kData->size() - rtCnt;
	ZTSBarStruct* rtHead = kData->at(sIdx);
	ZTSKlineSlice* slice = ZTSKlineSlice::create(stdCode, period, times, rtHead, rtCnt);
	return slice;
}
