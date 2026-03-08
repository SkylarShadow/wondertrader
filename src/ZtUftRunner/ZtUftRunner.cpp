/*!
 * /file ZtUftRunner.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#include "ZtUftRunner.h"
#include "../ZtUftCore/ShareManager.h"

#include "../ZtUftCore/ZtHelper.h"
#include "../ZtUftCore/UftStraContext.h"

#include "../Includes/ZTSVariant.hpp"
#include "../ZTSTools/ZTSLogger.h"
#include "../ZTSUtils/ZTSCfgLoader.h"
#include "../ZTSUtils/SignalHook.hpp"
#include "../Share/StrUtil.hpp"

const char* getBinDir()
{
	static std::string basePath;
	if (basePath.empty())
	{
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath.c_str();
}



ZtUftRunner::ZtUftRunner()
	:_to_exit(false)
{
	install_signal_hooks([](const char* message) {
		ZTSLogger::error(message);
	}, [this](bool bStopped) {
		_to_exit = bStopped;
		ZTSLogger::info("Exit flag is {}", _to_exit);
	});
}


ZtUftRunner::~ZtUftRunner()
{
}

void ZtUftRunner::init(const std::string& filename)
{
	ZTSLogger::init(filename.c_str());

	ZtHelper::setInstDir(getBinDir());
}

bool ZtUftRunner::config(const std::string& filename)
{
	_config = ZTSCfgLoader::load_from_file(filename.c_str());
	if(_config == NULL)
	{
		ZTSLogger::error("Loading config file {} failed", filename);
		return false;
	}

	//基础数据文件
	ZTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"));

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
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));

	//初始化运行环境
	initEngine();

	//初始化数据管理
	initDataMgr();

	if (_config->has("share_domain"))
	{
		ZTSVariant* cfg = _config->get("share_domain");
		ShareManager::self().set_engine(&_uft_engine);

		ShareManager::self().initialize(cfg->getCString("module"));
		ShareManager::self().init_domain(cfg->getCString("name"));
	}

	if(!_act_policy.init(_config->getCString("bspolicy")))
	{
		ZTSLogger::error("ActionPolicyMgr init failed, please check config");
	}

	//初始化行情通道
	ZTSVariant* cfgParser = _config->get("parsers");
	if (cfgParser)
	{
		if (cfgParser->type() == ZTSVariant::VT_String)
		{
			const char* filename = cfgParser->asCString();
			if (StdFile::exists(filename))
			{
				ZTSLogger::info("Reading parser config from {}...", filename);
				ZTSVariant* var = ZTSCfgLoader::load_from_file(filename);
				if(var)
				{
					if (!initParsers(var->get("parsers")))
						ZTSLogger::error("Loading parsers failed");
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

	//初始化交易通道
	ZTSVariant* cfgTraders = _config->get("traders");
	if (cfgTraders)
	{
		if (cfgTraders->type() == ZTSVariant::VT_String)
		{
			const char* filename = cfgTraders->asCString();
			if (StdFile::exists(filename))
			{
				ZTSLogger::info("Reading trader config from {}...", filename);
				ZTSVariant* var = ZTSCfgLoader::load_from_file(filename);
				if (var)
				{
					if (!initTraders(var->get("traders")))
						ZTSLogger::error("Loading traders failed");
					var->release();
				}
				else
				{
					ZTSLogger::error("Loading trader config {} failed", filename);
				}
			}
			else
			{
				ZTSLogger::error("Trader configuration {} not exists", filename);
			}
		}
		else if (cfgTraders->type() == ZTSVariant::VT_Array)
		{
			initTraders(cfgTraders);
		}
	}

	initUftStrategies();
	
	return true;
}

bool ZtUftRunner::initUftStrategies()
{
	ZTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != ZTSVariant::VT_Object)
		return false;

	cfg = cfg->get("uft");
	if (cfg == NULL || cfg->type() != ZTSVariant::VT_Array)
		return false;

	std::string path = ZtHelper::getCWD() + "uft/";
	_uft_stra_mgr.loadFactories(path.c_str());

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		ZTSVariant* cfgItem = cfg->get(idx);
		if(!cfgItem->getBoolean("active"))
			continue;
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		UftStrategyPtr stra = _uft_stra_mgr.createStrategy(name, id);
		if (stra == NULL)
		{
			ZTSLogger::error("UFT Strategy {} create failed", name);
			continue;
		}
		else
		{
			ZTSLogger::info("UFT Strategy {}({}) created", name, id);
		}

		stra->self()->init(cfgItem->get("params"));
		UftStraContext* ctx = new UftStraContext(&_uft_engine, id);
		ctx->set_strategy(stra->self());

		const char* traderid = cfgItem->getCString("trader");
		TraderAdapterPtr trader = _traders.getAdapter(traderid);
		if(trader)
		{
			ctx->setTrader(trader.get());
			trader->addSink(ctx);
		}
		else
		{
			ZTSLogger::error("Trader {} not exists, binding trader to UFT strategy failed", traderid);
		}

		_uft_engine.addContext(UftContextPtr(ctx));
	}

	return true;
}

bool ZtUftRunner::initEngine()
{
	ZTSVariant* cfg = _config->get("env");
	if (cfg == NULL)
		return false;

	ZTSLogger::info("Trading enviroment initialzied with engine: UFT");
	_uft_engine.init(cfg, &_bd_mgr, &_data_mgr, &_notifier);

	_uft_engine.set_adapter_mgr(&_traders);

	return true;
}


bool ZtUftRunner::initDataMgr()
{
	ZTSVariant*cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, &_uft_engine);
	ZTSLogger::info("Data manager initialized");

	return true;
}

bool ZtUftRunner::initParsers(ZTSVariant* cfgParser)
{
	if (cfgParser == NULL)
		return false;

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfgParser->size(); idx++)
	{
		ZTSVariant* cfgItem = cfgParser->get(idx);
		if(!cfgItem->getBoolean("active"))
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

		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->init(realid.c_str(), cfgItem, &_uft_engine, &_bd_mgr);
		_parsers.addAdapter(realid.c_str(), adapter);

		count++;
	}

	ZTSLogger::info("{} parsers loaded", count);
	return true;
}

bool ZtUftRunner::initTraders(ZTSVariant* cfgTrader)
{
	if (cfgTrader == NULL || cfgTrader->type() != ZTSVariant::VT_Array)
		return false;
	
	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfgTrader->size(); idx++)
	{
		ZTSVariant* cfgItem = cfgTrader->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		TraderAdapterPtr adapter(new TraderAdapter());
		adapter->init(id, cfgItem, &_bd_mgr, &_act_policy);

		_traders.addAdapter(id, adapter);

		count++;
	}

	ZTSLogger::info("{} traders loaded", count);

	return true;
}

bool ZtUftRunner::initEvtNotifier()
{
	ZTSVariant* cfg = _config->get("notifier");
	if (cfg == NULL || cfg->type() != ZTSVariant::VT_Object)
		return false;

	_notifier.init(cfg);

	return true;
}

void ZtUftRunner::run(bool bAsync /* = false */)
{
	try
	{
		_uft_engine.run();

		_parsers.run();
		_traders.run();

		ShareManager::self().start_watching(2);

		while(!_to_exit)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	catch (...)
	{
		print_stack_trace([](const char* message) {
			ZTSLogger::error(message);
		});
	}
}

const char* LOG_TAGS[] = {
	"all",
	"debug",
	"info",
	"warn",
	"error",
	"fatal",
	"none"
};

void ZtUftRunner::handleLogAppend(ZTSLogLevel ll, const char* msg)
{
}