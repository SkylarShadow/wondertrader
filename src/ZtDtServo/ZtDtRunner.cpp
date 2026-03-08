/*!
 * \file ZtDtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ZtDtRunner.h"

#include "../ZtDtCore/ZtHelper.h"
#include "../Includes/ZTSSessionInfo.hpp"
#include "../Includes/ZTSVariant.hpp"
#include "../Includes/ZTSDataDef.hpp"
#include "../Includes/ZTSContractInfo.hpp"

#include "../ZTSUtils/SignalHook.hpp"
#include "../ZTSUtils/ZTSCfgLoader.h"
#include "../ZTSTools/ZTSLogger.h"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/CodeHelper.hpp"

USING_NS_ZTP;

ZtDtRunner::ZtDtRunner()
	: _data_store(NULL)
	, _is_inited(false)
{
	install_signal_hooks([](const char* message) {
		ZTSLogger::error(message);
	});
}


ZtDtRunner::~ZtDtRunner()
{
}
#ifdef _MSC_VER
#include "../Common/mdump.h"
extern const char* getModuleName();
#endif

void ZtDtRunner::initialize(const char* cfgFile, bool isFile /* = true */, const char* modDir /* = "" */, const char* logCfg /* = "logcfg.yaml" */, 
			FuncOnTickCallback cbTick /* = NULL */, FuncOnBarCallback cbBar /* = NULL */)
{
	if(_is_inited)
	{
		ZTSLogger::error("ZtDtServo has already been initialized");
		return;
	}

	_cb_tick = cbTick;
	_cb_bar = cbBar;

	ZTSLogger::init(logCfg);
	ZtHelper::set_module_dir(modDir);

	ZTSVariant* config = isFile ? ZTSCfgLoader::load_from_file(cfgFile) : ZTSCfgLoader::load_from_content(cfgFile, false);
	if(config == NULL)
	{
		ZTSLogger::error("Loading config failed");
		ZTSLogger::log_raw(LL_INFO, cfgFile);
		return;
	}

	if(!config->getBoolean("disable_dump"))
	{
#ifdef _MSC_VER
		CMiniDumper::Enable(getModuleName(), true, ZtHelper::get_cwd());
#endif
	}
	//基础数据文件
	ZTSVariant* cfgBF = config->get("basefiles");
	if (cfgBF->get("session"))
	{
		_bd_mgr.loadSessions(cfgBF->getCString("session"));
		ZTSLogger::info("Trading sessions loaded");
	}

	ZTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == ZTSVariant::VT_String)
		{
			_bd_mgr.loadCommodities(cfgItem->asCString());
		}
		else if (cfgItem->type() == ZTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadCommodities(cfgItem->get(i)->asCString());
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == ZTSVariant::VT_String)
		{
			_bd_mgr.loadContracts(cfgItem->asCString());
		}
		else if (cfgItem->type() == ZTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadContracts(cfgItem->get(i)->asCString());
			}
		}
	}

	if (cfgBF->get("holiday"))
	{
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));
		ZTSLogger::info("Holidays loaded");
	}

	if (cfgBF->get("hot"))
	{
		_hot_mgr.loadHots(cfgBF->getCString("hot"));
		ZTSLogger::info("Hot rules loaded");
	}

	if (cfgBF->get("second"))
	{
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));
		ZTSLogger::info("Second rules loaded");
	}

	ZTSArray* ayContracts = _bd_mgr.getContracts();
	for (auto it = ayContracts->begin(); it != ayContracts->end(); it++)
	{
		ZTSContractInfo* cInfo = (ZTSContractInfo*)(*it);
		bool isHot = _hot_mgr.isHot(cInfo->getExchg(), cInfo->getCode());
		bool isSecond = _hot_mgr.isSecond(cInfo->getExchg(), cInfo->getCode());

		std::string hotCode = cInfo->getFullPid();
		if (isHot)
			hotCode += ".HOT";
		else if (isSecond)
			hotCode += ".2ND";
		else
			hotCode = "";

		cInfo->setHotFlag(isHot ? 1 : (isSecond ? 2 : 0), hotCode.c_str());
	}
	ayContracts->release();

	initDataMgr(config->get("data"));

	ZTSVariant* cfgParser = config->get("parsers");
	if (cfgParser)
	{
		if (cfgParser->type() == ZTSVariant::VT_String)
		{
			const char* filename = cfgParser->asCString();
			if (StdFile::exists(filename))
			{
				ZTSLogger::info("Reading parser config from {}...", filename);
				ZTSVariant* var = ZTSCfgLoader::load_from_file(filename);
				if (var)
				{
					initParsers(var->get("parsers"));
					var->release();
				}
				else
				{
					ZTSLogger::error("Loading parser config {} failed", filename);
				}
			}
			else
			{
				ZTSLogger::error("Parser configuration {} not exists", filename);
			}
		}
		else if (cfgParser->type() == ZTSVariant::VT_Array)
		{
			initParsers(cfgParser);
		}
	}
	else
		ZTSLogger::log_raw(LL_WARN, "No parsers config, skipped loading parsers");

	config->release();

	start();

	_is_inited = true;
}

void ZtDtRunner::initDataMgr(ZTSVariant* config)
{
	if (config == NULL)
		return;

	_data_mgr.init(config, this);

	ZTSLogger::info("Data manager initialized");
}

ZTSKlineSlice* ZtDtRunner::get_bars_by_range(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime /* = 0 */)
{
	if(!_is_inited)
	{
		ZTSLogger::error("ZtDtServo not initialized");
		return NULL;
	}

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	ZTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (basePeriod[0] == 'm')
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
		kp = KP_DAY;

	if (endTime == 0)
	{
		uint32_t curDate = TimeUtils::getCurDate();
		endTime = (uint64_t)curDate * 10000 + 2359;
	}

	return _data_mgr.get_kline_slice_by_range(stdCode, kp, realTimes, beginTime, endTime);
}

ZTSKlineSlice* ZtDtRunner::get_bars_by_date(const char* stdCode, const char* period, uint32_t uDate /* = 0 */)
{
	if (!_is_inited)
	{
		ZTSLogger::error("ZtDtServo not initialized");
		return NULL;
	}

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	ZTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (basePeriod[0] == 'm')
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
	{
		ZTSLogger::log_raw(LL_ERROR, "get_bars_by_date only supports minute period");
		return NULL;
	}

	if (uDate == 0)
	{
		uDate = TimeUtils::getCurDate();
	}

	return _data_mgr.get_kline_slice_by_date(stdCode, kp, realTimes, uDate);
}

ZTSTickSlice* ZtDtRunner::get_ticks_by_range(const char* stdCode, uint64_t beginTime, uint64_t endTime /* = 0 */)
{
	if (!_is_inited)
	{
		ZTSLogger::error("ZtDtServo not initialized");
		return NULL;
	}

	if(endTime == 0)
	{
		uint32_t curDate = TimeUtils::getCurDate();
		endTime = (uint64_t)curDate * 10000 + 2359;
	}
	return _data_mgr.get_tick_slices_by_range(stdCode, beginTime, endTime);
}

ZTSTickSlice* ZtDtRunner::get_ticks_by_date(const char* stdCode, uint32_t uDate /* = 0 */)
{
	if (!_is_inited)
	{
		ZTSLogger::error("ZtDtServo not initialized");
		return NULL;
	}

	return _data_mgr.get_tick_slice_by_date(stdCode, uDate);
}

ZTSKlineSlice* ZtDtRunner::get_bars_by_count(const char* stdCode, const char* period, uint32_t count, uint64_t endTime /* = 0 */)
{
	if (!_is_inited)
	{
		ZTSLogger::error("ZtDtServo not initialized");
		return NULL;
	}

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	ZTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (basePeriod[0] == 'm')
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
		kp = KP_DAY;

	if (endTime == 0)
	{
		uint32_t curDate = TimeUtils::getCurDate();
		endTime = (uint64_t)curDate * 10000 + 2359;
	}

	return _data_mgr.get_kline_slice_by_count(stdCode, kp, realTimes, count, endTime);
}

ZTSTickSlice* ZtDtRunner::get_ticks_by_count(const char* stdCode, uint32_t count, uint64_t endTime /* = 0 */)
{
	if (!_is_inited)
	{
		ZTSLogger::error("ZtDtServo not initialized");
		return NULL;
	}

	if (endTime == 0)
	{
		uint32_t curDate = TimeUtils::getCurDate();
		endTime = (uint64_t)curDate * 10000 + 2359;
	}
	return _data_mgr.get_tick_slice_by_count(stdCode, count, endTime);
}

ZTSKlineSlice* ZtDtRunner::get_sbars_by_date(const char* stdCode, uint32_t secs, uint32_t uDate /* = 0 */)
{
	if (!_is_inited)
	{
		ZTSLogger::error("ZtDtServo not initialized");
		return NULL;
	}

	return _data_mgr.get_skline_slice_by_date(stdCode, secs, uDate);
}

void ZtDtRunner::initParsers(ZTSVariant* cfg)
{
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		ZTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		// By Wesley @ 2021.12.14
		// 如果id为空，则生成自动id
		std::string realid = id;
		if (realid.empty())
		{
			static uint32_t auto_parserid = 1000;
			realid = StrUtil::printf("auto_parser_%u", auto_parserid++);
		}

		ParserAdapterPtr adapter(new ParserAdapter(&_bd_mgr, this));
		adapter->init(realid.c_str(), cfgItem);
		_parsers.addAdapter(realid.c_str(), adapter);
	}

	ZTSLogger::info("{} market data parsers loaded in total", _parsers.size());
}

void ZtDtRunner::start()
{
	_parsers.run();
}

void ZtDtRunner::proc_tick(ZTSTickData* curTick)
{
	ZTSContractInfo* cInfo = curTick->getContractInfo();
	if (cInfo == NULL)
	{
		cInfo = _bd_mgr.getContract(curTick->code(), curTick->exchg());
		curTick->setContractInfo(cInfo);
	}

	if (cInfo == NULL)
		return;

	ZTSCommodityInfo* commInfo = cInfo->getCommInfo();
	ZTSSessionInfo* sInfo = commInfo->getSessionInfo();

	uint32_t hotflag = 0;

	std::string stdCode;
	if (commInfo->getCategoty() == CC_FutOption)
	{
		stdCode = CodeHelper::rawFutOptCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
	}
	else if (CodeHelper::isMonthlyCode(curTick->code()))
	{
		//如果是分月合约，则进行主力和次主力的判断
		stdCode = CodeHelper::rawMonthCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
	}
	else
	{
		stdCode = CodeHelper::rawFlatCodeToStdCode(cInfo->getCode(), cInfo->getExchg(), cInfo->getProduct());
	}
	curTick->setCode(stdCode.c_str());

	trigger_tick(stdCode.c_str(), curTick);

	if (!cInfo->isFlat())
	{
		const char* hotCode = cInfo->getHotCode();
		ZTSTickData* hotTick = ZTSTickData::create(curTick->getTickStruct());
		hotTick->setCode(hotCode);
		hotTick->setContractInfo(curTick->getContractInfo());

		trigger_tick(hotCode, hotTick);

		hotTick->release();
	}
	//else if (hotflag == 2)
	//{
	//	std::string scndCode = CodeHelper::stdCodeToStd2ndCode(stdCode.c_str());
	//	ZTSTickData* scndTick = ZTSTickData::create(curTick->getTickStruct());
	//	scndTick->setCode(scndCode.c_str());
	//	scndTick->setContractInfo(curTick->getContractInfo());

	//	trigger_tick(scndCode.c_str(), scndTick);

	//	scndTick->release();
	//}
}

void ZtDtRunner::trigger_tick(const char* stdCode, ZTSTickData* curTick)
{
	if (_cb_tick != NULL)
	{
		StdUniqueLock lock(_mtx_subs);
		auto sit = _tick_sub_map.find(stdCode);
		if (sit != _tick_sub_map.end())
		{
			SubFlags flags = sit->second;
			for (uint32_t flag : flags)
			{
				if (flag == 0)
				{
					_cb_tick(stdCode, &curTick->getTickStruct());
				}
				else
				{
					std::string wCode = fmtutil::format("{}{}", stdCode, (flag == 1) ? SUFFIX_QFQ : SUFFIX_HFQ);
					if (flag == 1)
					{
						_cb_tick(wCode.c_str(), &curTick->getTickStruct());
					}
					else //(flag == 2)
					{
						ZTSTickData* newTick = ZTSTickData::create(curTick->getTickStruct());
						ZTSTickStruct& newTS = newTick->getTickStruct();
						newTick->setContractInfo(curTick->getContractInfo());

						//这里做一个复权因子的处理
						double factor = _data_mgr.get_exright_factor(stdCode, curTick->getContractInfo()->getCommInfo());
						newTS.open *= factor;
						newTS.high *= factor;
						newTS.low *= factor;
						newTS.price *= factor;

						newTS.settle_price *= factor;

						newTS.pre_close *= factor;
						newTS.pre_settle *= factor;

						_cb_tick(wCode.c_str(), &newTS);
						newTick->release();
					}
				}
			}

		}
	}

	{
		StdUniqueLock lock(_mtx_innersubs);
		auto sit = _tick_innersub_map.find(stdCode);
		if (sit == _tick_innersub_map.end())
			return;

		SubFlags flags = sit->second;
		for (uint32_t flag : flags)
		{
			if (flag == 0)
			{
				_data_mgr.update_bars(stdCode, curTick);
			}
			else
			{
				std::string wCode = fmtutil::format("{}{}", stdCode, (flag == 1) ? SUFFIX_QFQ : SUFFIX_HFQ);
				curTick->setCode(wCode.c_str());
				if (flag == 1)
				{
					_data_mgr.update_bars(wCode.c_str(), curTick);
				}
				else //(flag == 2)
				{
					ZTSTickData* newTick = ZTSTickData::create(curTick->getTickStruct());
					ZTSTickStruct& newTS = newTick->getTickStruct();
					newTick->setContractInfo(curTick->getContractInfo());

					//这里做一个复权因子的处理
					double factor = _data_mgr.get_exright_factor(stdCode, curTick->getContractInfo()->getCommInfo());
					newTS.open *= factor;
					newTS.high *= factor;
					newTS.low *= factor;
					newTS.price *= factor;

					newTS.settle_price *= factor;

					newTS.pre_close *= factor;
					newTS.pre_settle *= factor;

					_data_mgr.update_bars(wCode.c_str(), newTick);
					newTick->release();
				}
			}
		}
	}
}

void ZtDtRunner::sub_tick(const char* codes, bool bReplace, bool bInner /* = false */)
{
	if(bInner)
	{
		StdUniqueLock lock(_mtx_innersubs);
		if (bReplace)
			_tick_innersub_map.clear();

		const char* stdCode = codes;
		std::size_t length = strlen(stdCode);
		uint32_t flag = 0;
		if (stdCode[length - 1] == SUFFIX_QFQ || stdCode[length - 1] == SUFFIX_HFQ)
		{
			length--;

			flag = (stdCode[length] == SUFFIX_QFQ) ? 1 : 2;
		}

		SubFlags& flags = _tick_innersub_map[std::string(stdCode, length)];
		flags.insert(flag);
		ZTSLogger::info("Tick dada of {} subscribed with flag {} for inner use", stdCode, flag);
	}
	else
	{
		StdUniqueLock lock(_mtx_subs);
		if (bReplace)
			_tick_sub_map.clear();

		StringVector ayCodes = StrUtil::split(codes, ",");
		for (const std::string& code : ayCodes)
		{
			//如果是主力合约代码, 如SHFE.ag.HOT, 那么要转换成原合约代码, SHFE.ag.1912
			//因为执行器只识别原合约代码
			const char* stdCode = code.c_str();
			std::size_t length = strlen(stdCode);
			uint32_t flag = 0;
			if (stdCode[length - 1] == SUFFIX_QFQ || stdCode[length - 1] == SUFFIX_HFQ)
			{
				length--;

				flag = (stdCode[length] == SUFFIX_QFQ) ? 1 : 2;
			}

			SubFlags& flags = _tick_sub_map[std::string(stdCode, length)];
			flags.insert(flag);
			ZTSLogger::info("Tick dada of {} subscribed with flag {}", stdCode, flag);
		}
	}
}

void ZtDtRunner::sub_bar(const char* stdCode, const char* period)
{
	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	ZTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (basePeriod[0] == 'm')
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
		kp = KP_DAY;

	_data_mgr.clear_subbed_bars();
	_data_mgr.subscribe_bar(stdCode, kp, realTimes);
	sub_tick(stdCode, true, true);
}

void ZtDtRunner::trigger_bar(const char* stdCode, const char* period, ZTSBarStruct* lastBar)
{
	if (_cb_bar == NULL)
		return;

	_cb_bar(stdCode, period, lastBar);
}

void ZtDtRunner::clear_cache()
{
	_data_mgr.clear_cache();
}