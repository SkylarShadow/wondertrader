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
#include "ExpParser.h"

#include "../ZtDtCore/ZtHelper.h"

#include "../Includes/ZTSSessionInfo.hpp"
#include "../Includes/ZTSVariant.hpp"
#include "../Includes/ZTSDataDef.hpp"
#include "../Includes/ZTSContractInfo.hpp"

#include "../Share/StrUtil.hpp"

#include "../ZTSUtils/ZTSCfgLoader.h"
#include "../ZTSTools/ZTSLogger.h"
#include "../ZTSUtils/SignalHook.hpp"


ZtDtRunner::ZtDtRunner()
	: _dumper_for_bars(NULL)
	, _dumper_for_ticks(NULL)
	, _dumper_for_ordque(NULL)
	, _dumper_for_orddtl(NULL)
	, _dumper_for_trans(NULL)
	, _to_exit(false)
{
}


ZtDtRunner::~ZtDtRunner()
{
}

void ZtDtRunner::start(bool bAsync /* = false */, bool bAlldayMode /* = false */)
{
	_parsers.run();

    if(!bAsync)
    {
		install_signal_hooks([this](const char* message) {
			if(!_to_exit)
				ZTSLogger::error(message);
		}, [this](bool toExit) {
			if (_to_exit)
				return;
			_to_exit = toExit;
			ZTSLogger::info("Exit flag is {}", _to_exit);
		});

		_async_io.post([this, bAlldayMode]() {
			if(!bAlldayMode)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				_state_mon.run();
			}
		});

		StdThread trd([this] {
			while (!_to_exit)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				_async_io.run_one();
			}
		});

		trd.join();
    }
	else
	{
		if (!bAlldayMode)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			_state_mon.run();
		}
	}
}

void ZtDtRunner::initialize(const char* cfgFile, const char* logCfg, const char* modDir /* = "" */, bool bCfgFile /* = true */, bool bLogCfgFile /* = true */)
{
	ZTSLogger::init(logCfg, bLogCfgFile);
	ZtHelper::set_module_dir(modDir);

	ZTSVariant* config = NULL;
	if (bCfgFile)
		config = ZTSCfgLoader::load_from_file(cfgFile);
	else
		config = ZTSCfgLoader::load_from_content(cfgFile, false);

	if(config == NULL)
	{
		ZTSLogger::error("Loading config file {} failed", cfgFile);
		return;
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
		ZTSLogger::log_raw(LL_INFO, "Hot rules loaded");
	}

	if (cfgBF->get("second"))
	{
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));
		ZTSLogger::log_raw(LL_INFO, "Second rules loaded");
	}

	if (cfgBF->has("rules"))
	{
		auto cfgRules = cfgBF->get("rules");
		auto tags = cfgRules->memberNames();
		for (const std::string& ruleTag : tags)
		{
			_hot_mgr.loadCustomRules(ruleTag.c_str(), cfgRules->getCString(ruleTag.c_str()));
			ZTSLogger::info("{} rules loaded from {}", ruleTag, cfgRules->getCString(ruleTag.c_str()));
		}
	}

	if (config->has("shmcaster"))
	{
		_shm_caster.init(config->get("shmcaster"));
		_data_mgr.add_caster(&_shm_caster);
	}

	if(config->has("broadcaster"))
	{
		_udp_caster.init(config->get("broadcaster"), &_bd_mgr, &_data_mgr);
		_data_mgr.add_caster(&_udp_caster);
	}

	//By Wesley @ 2021.12.27
	//全天候模式，不需要再使用状态机
	bool bAlldayMode = config->getBoolean("allday");
	if (!bAlldayMode)
	{
		_state_mon.initialize(config->getCString("statemonitor"), &_bd_mgr, &_data_mgr);
	}
	else
	{
		ZTSLogger::info("datakit will run in allday mode");
	}

	initDataMgr(config->get("writer"), bAlldayMode);

	if (config->has("index"))
	{
		//如果存在指数模块要，配置指数
		const char* filename = config->getCString("index");
		ZTSLogger::info("Reading index config from {}...", filename);
		ZTSVariant* var = ZTSCfgLoader::load_from_file(filename);
		if (var)
		{
			_idx_factory.init(var, &_hot_mgr, &_bd_mgr, &_data_mgr);
			var->release();
		}
		else
		{
			ZTSLogger::error("Loading index config {} failed", filename);
		}
	}

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
}

void ZtDtRunner::initDataMgr(ZTSVariant* config, bool bAlldayMode /* = false */)
{
	_data_mgr.init(config, &_bd_mgr, bAlldayMode ? NULL : &_state_mon);
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
		
		ParserAdapterPtr adapter(new ParserAdapter(&_bd_mgr, &_data_mgr, &_idx_factory));
		adapter->init(realid.c_str(), cfgItem);
		_parsers.addAdapter(realid.c_str(), adapter);
	}

	ZTSLogger::info("{} market data parsers loaded in total", _parsers.size());
}

#pragma region "Extended Parser"
void ZtDtRunner::registerParserPorter(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub)
{
	_cb_parser_evt = cbEvt;
	_cb_parser_sub = cbSub;

	ZTSLogger::info("Callbacks of Extented Parser registration done");
}

void ZtDtRunner::parser_init(const char* id)
{
	if (_cb_parser_evt)
		_cb_parser_evt(EVENT_PARSER_INIT, id);
}

void ZtDtRunner::parser_connect(const char* id)
{
	if (_cb_parser_evt)
		_cb_parser_evt(EVENT_PARSER_CONNECT, id);
}

void ZtDtRunner::parser_disconnect(const char* id)
{
	if (_cb_parser_evt)
		_cb_parser_evt(EVENT_PARSER_DISCONNECT, id);
}

void ZtDtRunner::parser_release(const char* id)
{
	if (_cb_parser_evt)
		_cb_parser_evt(EVENT_PARSER_RELEASE, id);
}

void ZtDtRunner::parser_subscribe(const char* id, const char* code)
{
	if (_cb_parser_sub)
		_cb_parser_sub(id, code, true);
}

void ZtDtRunner::parser_unsubscribe(const char* id, const char* code)
{
	if (_cb_parser_sub)
		_cb_parser_sub(id, code, false);
}

void ZtDtRunner::on_ext_parser_quote(const char* id, ZTSTickStruct* curTick, uint32_t uProcFlag)
{
	ParserAdapterPtr adapter = _parsers.getAdapter(id);
	if (adapter)
	{
		ZTSTickData* newTick = ZTSTickData::create(*curTick);
		adapter->handleQuote(newTick, uProcFlag);
		newTick->release();
	}
	else
	{
		ZTSLogger::warn("Parser {} not exists", id);
	}
}

bool ZtDtRunner::createExtParser(const char* id)
{
	ParserAdapterPtr adapter(new ParserAdapter(&_bd_mgr, &_data_mgr, &_idx_factory));
	ExpParser* parser = new ExpParser(id);
	adapter->initExt(id, parser);
	_parsers.addAdapter(id, adapter);
	ZTSLogger::info("Extended parser {} created", id);
	return true;
}

#pragma endregion 

bool ZtDtRunner::createExtDumper(const char* id)
{
	ExpDumperPtr dumper(new ExpDumper(id));
	_dumpers[id] = dumper;

	_data_mgr.add_ext_dumper(id, dumper.get());

	ZTSLogger::info("Extended dumper {} created", id);
	return true;
}

void ZtDtRunner::registerExtDumper(FuncDumpBars barDumper, FuncDumpTicks tickDumper)
{
	_dumper_for_bars = barDumper;
	_dumper_for_ticks = tickDumper;
}

void ZtDtRunner::registerExtHftDataDumper(FuncDumpOrdQue ordQueDumper, FuncDumpOrdDtl ordDtlDumper, FuncDumpTrans transDumper)
{
	_dumper_for_ordque = ordQueDumper;
	_dumper_for_orddtl = ordDtlDumper;
	_dumper_for_trans = transDumper;
}

bool ZtDtRunner::dumpHisTicks(const char* id, const char* stdCode, uint32_t uDate, ZTSTickStruct* ticks, uint32_t count)
{
	if (NULL == _dumper_for_ticks)
	{
		ZTSLogger::error("Extended tick dumper not enabled");
		return false;
	}

	return _dumper_for_ticks(id, stdCode, uDate, ticks, count);
}

bool ZtDtRunner::dumpHisBars(const char* id, const char* stdCode, const char* period, ZTSBarStruct* bars, uint32_t count)
{
	if (NULL == _dumper_for_bars)
	{
		ZTSLogger::error("Extended bar dumper not enabled");
		return false;
	}

	return _dumper_for_bars(id, stdCode, period, bars, count);
}

bool ZtDtRunner::dumpHisOrdDtl(const char* id, const char* stdCode, uint32_t uDate, ZTSOrdDtlStruct* items, uint32_t count)
{
	if (NULL == _dumper_for_orddtl)
	{
		ZTSLogger::error("Extended order detail dumper not enabled");
		return false;
	}

	return _dumper_for_orddtl(id, stdCode, uDate, items, count);
}

bool ZtDtRunner::dumpHisOrdQue(const char* id, const char* stdCode, uint32_t uDate, ZTSOrdQueStruct* items, uint32_t count)
{
	if (NULL == _dumper_for_ordque)
	{
		ZTSLogger::error("Extended order queue dumper not enabled");
		return false;
	}

	return _dumper_for_ordque(id, stdCode, uDate, items, count);
}

bool ZtDtRunner::dumpHisTrans(const char* id, const char* stdCode, uint32_t uDate, ZTSTransStruct* items, uint32_t count)
{
	if (NULL == _dumper_for_trans)
	{
		ZTSLogger::error("Extended transaction dumper not enabled");
		return false;
	}

	return _dumper_for_trans(id, stdCode, uDate, items, count);
}