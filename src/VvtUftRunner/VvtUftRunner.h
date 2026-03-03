/*!
 * /file VvtUftRunner.h
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

#include "../VvtUftCore/EventNotifier.h"
#include "../VvtUftCore/UftStrategyMgr.h"

#include "../VvtUftCore/VvtUftEngine.h"
#include "../VvtUftCore/TraderAdapter.h"
#include "../VvtUftCore/ParserAdapter.h"
#include "../VvtUftCore/VvtUftDtMgr.h"
#include "../VvtUftCore/ActionPolicyMgr.h"

#include "../VvTSTools/VvTSHotMgr.h"
#include "../VvTSTools/VvTSBaseDataMgr.h"

NS_VVTP_BEGIN
class VvTSVariant;
NS_VVTP_END

USING_NS_VVTP;

class VvtUftRunner : public ILogHandler
{
public:
	VvtUftRunner();
	~VvtUftRunner();

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

	VvtUftEngine			_uft_engine;

	VvtUftDtMgr			_data_mgr;

	VvTSBaseDataMgr		_bd_mgr;
	EventNotifier		_notifier;

	UftStrategyMgr		_uft_stra_mgr;

	ActionPolicyMgr		_act_policy;

	bool				_to_exit;
};

