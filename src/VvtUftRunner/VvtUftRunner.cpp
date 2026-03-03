/*!
 * /file VvtUftRunner.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#include "VvtUftRunner.h"
#include "../VvtUftCore/ShareManager.h"

#include "../VvtUftCore/VvtHelper.h"
#include "../VvtUftCore/UftStraContext.h"

#include "../Includes/VvTSVariant.hpp"
#include "../VvTSTools/VvTSLogger.h"
#include "../VvTSUtils/VvTSCfgLoader.h"
#include "../VvTSUtils/SignalHook.hpp"
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



VvtUftRunner::VvtUftRunner()
	:_to_exit(false)
{
	install_signal_hooks([](const char* message) {
		VvTSLogger::error(message);
	}, [this](bool bStopped) {
		_to_exit = bStopped;
		VvTSLogger::info("Exit flag is {}", _to_exit);
	});
}


VvtUftRunner::~VvtUftRunner()
{
}

void VvtUftRunner::init(const std::string& filename)
{
	VvTSLogger::init(filename.c_str());

	VvtHelper::setInstDir(getBinDir());
}

bool VvtUftRunner::config(const std::string& filename)
{
	_config = VvTSCfgLoader::load_from_file(filename.c_str());
	if(_config == NULL)
	{
		VvTSLogger::error("Loading config file {} failed", filename);
		return false;
	}

	//基础数据文件
	VvTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"));

	VvTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == VvTSVariant::VT_String)
		{
			_bd_mgr.loadCommodities(cfgItem->asCString());
		}
		else if (cfgItem->type() == VvTSVariant::VT_Array)
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
		if (cfgItem->type() == VvTSVariant::VT_String)
		{
			_bd_mgr.loadContracts(cfgItem->asCString());
		}
		else if (cfgItem->type() == VvTSVariant::VT_Array)
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
		VvTSVariant* cfg = _config->get("share_domain");
		ShareManager::self().set_engine(&_uft_engine);

		ShareManager::self().initialize(cfg->getCString("module"));
		ShareManager::self().init_domain(cfg->getCString("name"));
	}

	if(!_act_policy.init(_config->getCString("bspolicy")))
	{
		VvTSLogger::error("ActionPolicyMgr init failed, please check config");
	}

	//初始化行情通道
	VvTSVariant* cfgParser = _config->get("parsers");
	if (cfgParser)
	{
		if (cfgParser->type() == VvTSVariant::VT_String)
		{
			const char* filename = cfgParser->asCString();
			if (StdFile::exists(filename))
			{
				VvTSLogger::info("Reading parser config from {}...", filename);
				VvTSVariant* var = VvTSCfgLoader::load_from_file(filename);
				if(var)
				{
					if (!initParsers(var->get("parsers")))
						VvTSLogger::error("Loading parsers failed");
					var->release();
				}
				else
				{
					VvTSLogger::error("Loading parser config {} failed", filename);
				}
			}
			else
			{
				VvTSLogger::error("Parser configuration {} not exists", filename);
			}
		}
		else if (cfgParser->type() == VvTSVariant::VT_Array)
		{
			initParsers(cfgParser);
		}
	}

	//初始化交易通道
	VvTSVariant* cfgTraders = _config->get("traders");
	if (cfgTraders)
	{
		if (cfgTraders->type() == VvTSVariant::VT_String)
		{
			const char* filename = cfgTraders->asCString();
			if (StdFile::exists(filename))
			{
				VvTSLogger::info("Reading trader config from {}...", filename);
				VvTSVariant* var = VvTSCfgLoader::load_from_file(filename);
				if (var)
				{
					if (!initTraders(var->get("traders")))
						VvTSLogger::error("Loading traders failed");
					var->release();
				}
				else
				{
					VvTSLogger::error("Loading trader config {} failed", filename);
				}
			}
			else
			{
				VvTSLogger::error("Trader configuration {} not exists", filename);
			}
		}
		else if (cfgTraders->type() == VvTSVariant::VT_Array)
		{
			initTraders(cfgTraders);
		}
	}

	initUftStrategies();
	
	return true;
}

bool VvtUftRunner::initUftStrategies()
{
	VvTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != VvTSVariant::VT_Object)
		return false;

	cfg = cfg->get("uft");
	if (cfg == NULL || cfg->type() != VvTSVariant::VT_Array)
		return false;

	std::string path = VvtHelper::getCWD() + "uft/";
	_uft_stra_mgr.loadFactories(path.c_str());

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		VvTSVariant* cfgItem = cfg->get(idx);
		if(!cfgItem->getBoolean("active"))
			continue;
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		UftStrategyPtr stra = _uft_stra_mgr.createStrategy(name, id);
		if (stra == NULL)
		{
			VvTSLogger::error("UFT Strategy {} create failed", name);
			continue;
		}
		else
		{
			VvTSLogger::info("UFT Strategy {}({}) created", name, id);
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
			VvTSLogger::error("Trader {} not exists, binding trader to UFT strategy failed", traderid);
		}

		_uft_engine.addContext(UftContextPtr(ctx));
	}

	return true;
}

bool VvtUftRunner::initEngine()
{
	VvTSVariant* cfg = _config->get("env");
	if (cfg == NULL)
		return false;

	VvTSLogger::info("Trading enviroment initialzied with engine: UFT");
	_uft_engine.init(cfg, &_bd_mgr, &_data_mgr, &_notifier);

	_uft_engine.set_adapter_mgr(&_traders);

	return true;
}


bool VvtUftRunner::initDataMgr()
{
	VvTSVariant*cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, &_uft_engine);
	VvTSLogger::info("Data manager initialized");

	return true;
}

bool VvtUftRunner::initParsers(VvTSVariant* cfgParser)
{
	if (cfgParser == NULL)
		return false;

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfgParser->size(); idx++)
	{
		VvTSVariant* cfgItem = cfgParser->get(idx);
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

	VvTSLogger::info("{} parsers loaded", count);
	return true;
}

bool VvtUftRunner::initTraders(VvTSVariant* cfgTrader)
{
	if (cfgTrader == NULL || cfgTrader->type() != VvTSVariant::VT_Array)
		return false;
	
	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfgTrader->size(); idx++)
	{
		VvTSVariant* cfgItem = cfgTrader->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		TraderAdapterPtr adapter(new TraderAdapter());
		adapter->init(id, cfgItem, &_bd_mgr, &_act_policy);

		_traders.addAdapter(id, adapter);

		count++;
	}

	VvTSLogger::info("{} traders loaded", count);

	return true;
}

bool VvtUftRunner::initEvtNotifier()
{
	VvTSVariant* cfg = _config->get("notifier");
	if (cfg == NULL || cfg->type() != VvTSVariant::VT_Object)
		return false;

	_notifier.init(cfg);

	return true;
}

void VvtUftRunner::run(bool bAsync /* = false */)
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
			VvTSLogger::error(message);
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

void VvtUftRunner::handleLogAppend(VvTSLogLevel ll, const char* msg)
{
}