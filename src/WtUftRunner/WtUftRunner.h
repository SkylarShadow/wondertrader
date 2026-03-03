/*!
 * /file WtUftRunner.h
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

#include "../WtUftCore/EventNotifier.h"
#include "../WtUftCore/UftStrategyMgr.h"

#include "../WtUftCore/WtUftEngine.h"
#include "../WtUftCore/TraderAdapter.h"
#include "../WtUftCore/ParserAdapter.h"
#include "../WtUftCore/WtUftDtMgr.h"
#include "../WtUftCore/ActionPolicyMgr.h"

#include "../VvTSTools/VvTSHotMgr.h"
#include "../VvTSTools/VvTSBaseDataMgr.h"

NS_VVTP_BEGIN
class VvTSVariant;
NS_VVTP_END

USING_NS_VVTP;

class WtUftRunner : public ILogHandler
{
public:
	WtUftRunner();
	~WtUftRunner();

public:
	void init(const std::string& filename);

	bool config(const std::string& filename);

	void run(bool bAsync = false);

private:
	bool initTraders(VvTSVariant* cfgTrader);
	bool initParsers(VvTSVariant* cfgParser);
	bool initDataMgr();
	bool initUftStrategies();
	bool initEvtNotifier();
	bool initEngine();

//////////////////////////////////////////////////////////////////////////
//ILogHandler
public:
	virtual void handleLogAppend(VvTSLogLevel ll, const char* msg) override;

private:
	VvTSVariant*			_config;
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;

	WtUftEngine			_uft_engine;

	WtUftDtMgr			_data_mgr;

	VvTSBaseDataMgr		_bd_mgr;
	EventNotifier		_notifier;

	UftStrategyMgr		_uft_stra_mgr;

	ActionPolicyMgr		_act_policy;

	bool				_to_exit;
};

