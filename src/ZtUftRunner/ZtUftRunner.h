/*!
 * /file ZtUftRunner.h
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

#include "../ZtUftCore/EventNotifier.h"
#include "../ZtUftCore/UftStrategyMgr.h"

#include "../ZtUftCore/ZtUftEngine.h"
#include "../ZtUftCore/TraderAdapter.h"
#include "../ZtUftCore/ParserAdapter.h"
#include "../ZtUftCore/ZtUftDtMgr.h"
#include "../ZtUftCore/ActionPolicyMgr.h"

#include "../ZTSTools/ZTSHotMgr.h"
#include "../ZTSTools/ZTSBaseDataMgr.h"

NS_ZTP_BEGIN
class ZTSVariant;
NS_ZTP_END

USING_NS_ZTP;

class ZtUftRunner : public ILogHandler
{
public:
	ZtUftRunner();
	~ZtUftRunner();

public:
	void init(const std::string& filename);

	bool config(const std::string& filename);

	void run(bool bAsync = false);

private:
	bool initTraders(ZTSVariant* cfgTrader);
	bool initParsers(ZTSVariant* cfgParser);
	bool initDataMgr();
	bool initUftStrategies();
	bool initEvtNotifier();
	bool initEngine();

//////////////////////////////////////////////////////////////////////////
//ILogHandler
public:
	virtual void handleLogAppend(ZTSLogLevel ll, const char* msg) override;

private:
	ZTSVariant*			_config;
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;

	ZtUftEngine			_uft_engine;

	ZtUftDtMgr			_data_mgr;

	ZTSBaseDataMgr		_bd_mgr;
	EventNotifier		_notifier;

	UftStrategyMgr		_uft_stra_mgr;

	ActionPolicyMgr		_act_policy;

	bool				_to_exit;
};

