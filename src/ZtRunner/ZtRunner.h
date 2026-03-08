/*!
 * /file ZtRunner.h
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#pragma once
#include <string>
#include <unordered_map>

#include "../Includes/ILogHandler.h"

#include "../ZtCore/EventNotifier.h"
#include "../ZtCore/CtaStrategyMgr.h"
#include "../ZtCore/HftStrategyMgr.h"
#include "../ZtCore/SelStrategyMgr.h"

#include "../ZtCore/ZtCtaEngine.h"
#include "../ZtCore/ZtHftEngine.h"
#include "../ZtCore/ZtSelEngine.h"
#include "../ZtCore/ZtLocalExecuter.h"
#include "../ZtCore/ZtDistExecuter.h"
#include "../ZtCore/TraderAdapter.h"
#include "../ZtCore/ParserAdapter.h"
#include "../ZtCore/ZtDtMgr.h"
#include "../ZtCore/ActionPolicyMgr.h"

#include "../ZTSTools/ZTSHotMgr.h"
#include "../ZTSTools/ZTSBaseDataMgr.h"

NS_ZTP_BEGIN
class ZTSVariant;
class ZtDataStorage;
NS_ZTP_END

USING_NS_ZTP;

class ZtRunner : public ILogHandler
{
public:
	ZtRunner();
	~ZtRunner();

public:
	void init(const std::string& filename);

	bool config(const std::string& filename);

	void run(bool bAsync = false);

private:
	bool initTraders(ZTSVariant* cfgTrader);
	bool initParsers(ZTSVariant* cfgParser);
	bool initExecuters(ZTSVariant* cfgExecuter);
	bool initDataMgr();
	bool initEvtNotifier();
	bool initCtaStrategies();
	bool initHftStrategies();
	bool initActionPolicy();

	bool initEngine();

//////////////////////////////////////////////////////////////////////////
//ILogHandler
public:
	virtual void handleLogAppend(ZTSLogLevel ll, const char* msg) override;

private:
	ZTSVariant*			_config;
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;
	ZtExecuterFactory	_exe_factory;

	ZtCtaEngine			_cta_engine;
	ZtHftEngine			_hft_engine;
	ZtSelEngine			_sel_engine;
	ZtEngine*			_engine;

	ZtDataStorage*		_data_store;

	ZtDtMgr				_data_mgr;

	ZTSBaseDataMgr		_bd_mgr;
	ZTSHotMgr			_hot_mgr;
	EventNotifier		_notifier;

	CtaStrategyMgr		_cta_stra_mgr;
	HftStrategyMgr		_hft_stra_mgr;
	SelStrategyMgr		_sel_stra_mgr;
	ActionPolicyMgr		_act_policy;

	bool				_is_hft;
	bool				_is_sel;

	bool				_to_exit;
};

