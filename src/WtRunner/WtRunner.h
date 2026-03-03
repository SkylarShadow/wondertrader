/*!
 * /file WtRunner.h
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

#include "../VvtCore/EventNotifier.h"
#include "../VvtCore/CtaStrategyMgr.h"
#include "../VvtCore/HftStrategyMgr.h"
#include "../VvtCore/SelStrategyMgr.h"

#include "../VvtCore/VvtCtaEngine.h"
#include "../VvtCore/VvtHftEngine.h"
#include "../VvtCore/VvtSelEngine.h"
#include "../VvtCore/VvtLocalExecuter.h"
#include "../VvtCore/VvtDistExecuter.h"
#include "../VvtCore/TraderAdapter.h"
#include "../VvtCore/ParserAdapter.h"
#include "../VvtCore/VvtDtMgr.h"
#include "../VvtCore/ActionPolicyMgr.h"

#include "../VvTSTools/VvTSHotMgr.h"
#include "../VvTSTools/VvTSBaseDataMgr.h"

NS_VVTP_BEGIN
class VvTSVariant;
class VvtDataStorage;
NS_VVTP_END

USING_NS_VVTP;

class WtRunner : public ILogHandler
{
public:
	WtRunner();
	~WtRunner();

public:
	void init(const std::string& filename);

	bool config(const std::string& filename);

	void run(bool bAsync = false);

private:
	bool initTraders(VvTSVariant* cfgTrader);
	bool initParsers(VvTSVariant* cfgParser);
	bool initExecuters(VvTSVariant* cfgExecuter);
	bool initDataMgr();
	bool initEvtNotifier();
	bool initCtaStrategies();
	bool initHftStrategies();
	bool initActionPolicy();

	bool initEngine();

//////////////////////////////////////////////////////////////////////////
//ILogHandler
public:
	virtual void handleLogAppend(VvTSLogLevel ll, const char* msg) override;

private:
	VvTSVariant*			_config;
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;
	VvtExecuterFactory	_exe_factory;

	VvtCtaEngine			_cta_engine;
	VvtHftEngine			_hft_engine;
	VvtSelEngine			_sel_engine;
	VvtEngine*			_engine;

	VvtDataStorage*		_data_store;

	VvtDtMgr				_data_mgr;

	VvTSBaseDataMgr		_bd_mgr;
	VvTSHotMgr			_hot_mgr;
	EventNotifier		_notifier;

	CtaStrategyMgr		_cta_stra_mgr;
	HftStrategyMgr		_hft_stra_mgr;
	SelStrategyMgr		_sel_stra_mgr;
	ActionPolicyMgr		_act_policy;

	bool				_is_hft;
	bool				_is_sel;

	bool				_to_exit;
};

