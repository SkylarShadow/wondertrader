#include <iostream>
#include <boost/filesystem.hpp>

#include "../Includes/IParserApi.h"
#include "../Includes/ZTSVariant.hpp"
#include "../Includes/ZTSContractInfo.hpp"
#include "../Includes/ZTSDataDef.hpp"

#include "../Share/DLLHelper.hpp"
#include "../Share/StrUtil.hpp"

#include "../ZTSTools/ZTSBaseDataMgr.h"
#include "../ZTSTools/ZTSLogger.h"
#include "../ZTSUtils/ZTSCfgLoader.h"

ZTSBaseDataMgr	g_bdMgr;

USING_NS_ZTP;

class ParserSpi : public IParserSpi
{
public:
	ParserSpi(){}

	bool init(ZTSVariant* params, const char* ttype)
	{
		m_pParams = params;
		if (m_pParams)
			m_pParams->retain();

		m_strModule = ttype;
		return true;
	}

	void release()
	{
		if (_api)
		{
			_api->release();
		}
	}

	void run(bool bRestart = false)
	{
		if (!createParser(m_strModule.c_str()))
		{
			return;
		}

		_api->registerSpi(this);

		if (!_api->init(m_pParams))
		{
			return;
		}

		ContractSet contractSet;
		ZTSArray* ayContract = g_bdMgr.getContracts();
		ZTSArray::Iterator it = ayContract->begin();
		for (; it != ayContract->end(); it++)
		{
			ZTSContractInfo* contract = STATIC_CONVERT(*it, ZTSContractInfo*);
			contractSet.insert(contract->getFullCode());
		}

		ayContract->release();
		_api->subscribe(contractSet);
		_api->connect();
	}

	bool createParser(const char* moduleName)
	{
        DllHandle hInst = DLLHelper::load_library(moduleName);
		if (hInst == NULL)
		{
			ZTSLogger::error("Loading module {} failed", moduleName);
			return false;
		}

		FuncCreateParser pCreator = (FuncCreateParser)DLLHelper::get_symbol(hInst, "createParser");
		if (NULL == pCreator)
		{
			ZTSLogger::error("Entry function createParser not found");
			return false;
		}

		_api = pCreator();
		if (NULL == _api)
		{
			ZTSLogger::error("Creating parser api failed");
			return false;
		}

		m_funcRemover = (FuncDeleteParser)DLLHelper::get_symbol(hInst, "deleteParser");
		return true;
	}

public:
	virtual void handleParserLog(ZTSLogLevel ll, const char* message) override
	{
		ZTSLogger::log_raw(ll, message);
	}

	virtual void handleQuote(ZTSTickData *quote, uint32_t procFlag) override
	{
		ZTSLogger::info("{}@{}.{}, price:{}, voume:{}", quote->code(), quote->actiondate(), quote->actiontime(), quote->price(), quote->totalvolume());
	}

	virtual void handleSymbolList(const ZTSArray* aySymbols) override
	{

	}

public:
	virtual IBaseDataMgr*	getBaseDataMgr()
	{
		return &g_bdMgr;
	}
	

private:
	IParserApi*			_api;
	FuncDeleteParser	m_funcRemover;
	std::string			m_strModule;
	ZTSVariant*			m_pParams;
};

std::string getBaseFolder()
{
	static std::string basePath;
	if (basePath.empty())
	{
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath.c_str();
}

int main()
{
	ZTSLogger::init("logcfg.yaml");

	ZTSVariant* root = ZTSCfgLoader::load_from_file("config.yaml");
	if (root == NULL)
	{
		ZTSLogger::log_raw(LL_ERROR, "Loading config.yaml failed");
		return 0;
	}

	ZTSVariant* cfg = root->get("config");
	if (cfg->has("session"))
		g_bdMgr.loadSessions(cfg->getCString("session"));

	if (cfg->has("commodity"))
		g_bdMgr.loadCommodities(cfg->getCString("commodity"));

	if (cfg->has("contract"))
		g_bdMgr.loadContracts(cfg->getCString("contract"));

	std::string module = cfg->getCString("parser");
	std::string profile = cfg->getCString("profile");
	ZTSVariant* params = root->get(profile.c_str());
	if (params == NULL)
	{
		ZTSLogger::error("Configure {} not exist", profile);
		return 0;
	}

	ParserSpi* parser = new ParserSpi;
	parser->init(params, module.c_str());
	parser->run();

	root->release();

	getchar();
	
	//exit(9);
	parser->release();
	delete parser;

	return 0;
}