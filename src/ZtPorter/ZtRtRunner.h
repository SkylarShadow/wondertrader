/*!
 * \file ZtRtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>

#include "PorterDefs.h"

#include "../Includes/ILogHandler.h"
#include "../Includes/IDataReader.h"

#include "../ZtCore/EventNotifier.h"
#include "../ZtCore/CtaStrategyMgr.h"
#include "../ZtCore/HftStrategyMgr.h"
#include "../ZtCore/SelStrategyMgr.h"
#include "../ZtCore/ZtCtaEngine.h"
#include "../ZtCore/ZtHftEngine.h"
#include "../ZtCore/ZtSelEngine.h"
#include "../ZtCore/ZtLocalExecuter.h"
#include "../ZtCore/ZtDiffExecuter.h"
#include "../ZtCore/ZtDistExecuter.h"
#include "../ZtCore/ZtArbiExecuter.h"
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

typedef enum tagEngineType
{
	ET_CTA = 999,	//CTA引擎	
	ET_HFT,			//高频引擎
	ET_SEL			//选股引擎
} EngineType;

class ZtRtRunner : public IEngineEvtListener, public ILogHandler, public IHisDataLoader
{
public:
	ZtRtRunner();
	~ZtRtRunner();

public:
	//////////////////////////////////////////////////////////////////////////
	//IBtDataLoader
	virtual bool loadFinalHisBars(void* obj, const char* stdCode, ZTSKlinePeriod period, FuncReadBars cb) override;

	virtual bool loadRawHisBars(void* obj, const char* stdCode, ZTSKlinePeriod period, FuncReadBars cb) override;

	virtual bool loadAllAdjFactors(void* obj, FuncReadFactors cb) override;

	virtual bool loadAdjFactors(void* obj, const char* stdCode, FuncReadFactors cb) override;

	void feedRawBars(ZTSBarStruct* bars, uint32_t count);

	void feedAdjFactors(const char* stdCode, uint32_t* dates, double* factors, uint32_t count);

public:
	/*
	 *	初始化
	 */
	bool init(const char* logCfg = "logcfg.prop", bool isFile = true, const char* genDir = "");

	bool config(const char* cfgFile, bool isFile = true);

	void run(bool bAsync = false);

	void release();

	void registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt, FuncStraCondTriggerCallback cbCondTrigger = NULL);
	void registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt);
	void registerHftCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar,
		FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust,
		FuncStraOrdDtlCallback cbOrdDtl, FuncStraOrdQueCallback cbOrdQue, FuncStraTransCallback cbTrans, FuncSessionEvtCallback cbSessEvt, FuncHftPosCallback cbPosition);

	void registerEvtCallback(FuncEventCallback cbEvt);

	void registerParserPorter(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub);

	void registerExecuterPorter(FuncExecInitCallback cbInit, FuncExecCmdCallback cbExec);

	void		registerExtDataLoader(FuncLoadFnlBars fnlBarLoader, FuncLoadRawBars rawBarLoader, FuncLoadAdjFactors fctLoader, FuncLoadRawTicks tickLoader = NULL)
	{
		_ext_fnl_bar_loader = fnlBarLoader;
		_ext_raw_bar_loader = rawBarLoader;
		_ext_adj_fct_loader = fctLoader;
	}

	bool			createExtParser(const char* id);
	bool			createExtExecuter(const char* id);

	uint32_t		createCtaContext(const char* name, int32_t slippage);
	uint32_t		createHftContext(const char* name, const char* trader, bool bAgent, int32_t slippage);
	uint32_t		createSelContext(const char* name, uint32_t date, uint32_t time, const char* period, int32_t slippage, const char* trdtpl = "CHINA", const char* session="TRADING");

	CtaContextPtr	getCtaContext(uint32_t id);
	SelContextPtr	getSelContext(uint32_t id);
	HftContextPtr	getHftContext(uint32_t id);
	ZtEngine*		getEngine(){ return _engine; }

	const char*	get_raw_stdcode(const char* stdCode);

//////////////////////////////////////////////////////////////////////////
//ILogHandler
public:
	virtual void handleLogAppend(ZTSLogLevel ll, const char* msg) override;

//////////////////////////////////////////////////////////////////////////
//扩展Parser
public:
	void parser_init(const char* id);
	void parser_connect(const char* id);
	void parser_release(const char* id);
	void parser_disconnect(const char* id);
	void parser_subscribe(const char* id, const char* code);
	void parser_unsubscribe(const char* id, const char* code);

	void on_ext_parser_quote(const char* id, ZTSTickStruct* curTick, uint32_t uProcFlag);


//////////////////////////////////////////////////////////////////////////
//扩展Executer
public:
	void executer_set_position(const char* id, const char* stdCode, double target);
	void executer_init(const char* id);

//////////////////////////////////////////////////////////////////////////
//ICtaEventListener
public:
	virtual void on_initialize_event() override
	{
		if (_cb_evt)
			_cb_evt(EVENT_ENGINE_INIT, 0, 0);
	}

	virtual void on_schedule_event(uint32_t uDate, uint32_t uTime) override
	{
		if (_cb_evt)
			_cb_evt(EVENT_ENGINE_SCHDL, uDate, uTime);
	}

	virtual void on_session_event(uint32_t uDate, bool isBegin = true) override
	{
		if (_cb_evt)
			_cb_evt(isBegin ? EVENT_SESSION_BEGIN : EVENT_SESSION_END, uDate, 0);
	}

public:
	void ctx_on_init(uint32_t id, EngineType eType = ET_CTA);
	void ctx_on_session_event(uint32_t id, uint32_t curTDate, bool isBegin = true, EngineType eType = ET_CTA);
	void ctx_on_tick(uint32_t id, const char* stdCode, ZTSTickData* newTick, EngineType eType = ET_CTA);
	void ctx_on_calc(uint32_t id, uint32_t curDate, uint32_t curTime, EngineType eType = ET_CTA);
	void ctx_on_bar(uint32_t id, const char* stdCode, const char* period, ZTSBarStruct* newBar, EngineType eType = ET_CTA);
	void ctx_on_cond_triggered(uint32_t id, const char* stdCode, double target, double price, const char* usertag, EngineType eType = ET_CTA);

	void hft_on_channel_ready(uint32_t cHandle, const char* trader);
	void hft_on_channel_lost(uint32_t cHandle, const char* trader);
	void hft_on_order(uint32_t cHandle, ZTUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag);
	void hft_on_trade(uint32_t cHandle, ZTUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag);
	void hft_on_entrust(uint32_t cHandle, ZTUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag);
	void hft_on_position(uint32_t cHandle, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail);

	void hft_on_order_queue(uint32_t id, const char* stdCode, ZTSOrdQueData* newOrdQue);
	void hft_on_order_detail(uint32_t id, const char* stdCode, ZTSOrdDtlData* newOrdDtl);
	void hft_on_transaction(uint32_t id, const char* stdCode, ZTSTransData* newTranns);

	bool addExeFactories(const char* folder);
	bool addCtaFactories(const char* folder);
	bool addHftFactories(const char* folder);
	bool addSelFactories(const char* folder);

private:
	bool initTraders(ZTSVariant* cfgTrader);
	bool initParsers(ZTSVariant* cfgParser);
	bool initExecuters(ZTSVariant* cfgExecuter);
	bool initDataMgr();
	bool initEvtNotifier();
	bool initCtaStrategies();
	bool initHftStrategies();
	bool initSelStrategies();
	bool initActionPolicy();

	bool initEngine();

private:
	FuncStraInitCallback	_cb_cta_init;
	FuncSessionEvtCallback	_cb_cta_sessevt;
	FuncStraTickCallback	_cb_cta_tick;
	FuncStraCalcCallback	_cb_cta_calc;
	FuncStraBarCallback		_cb_cta_bar;
	FuncStraCondTriggerCallback _cb_cta_cond_trigger;

	FuncStraInitCallback	_cb_sel_init;
	FuncSessionEvtCallback	_cb_sel_sessevt;
	FuncStraTickCallback	_cb_sel_tick;
	FuncStraCalcCallback	_cb_sel_calc;
	FuncStraBarCallback		_cb_sel_bar;

	FuncStraInitCallback	_cb_hft_init;
	FuncSessionEvtCallback	_cb_hft_sessevt;
	FuncStraTickCallback	_cb_hft_tick;
	FuncStraBarCallback		_cb_hft_bar;
	FuncHftChannelCallback	_cb_hft_chnl;
	FuncHftOrdCallback		_cb_hft_ord;
	FuncHftTrdCallback		_cb_hft_trd;
	FuncHftEntrustCallback	_cb_hft_entrust;
	FuncHftPosCallback		_cb_hft_position;

	FuncStraOrdQueCallback	_cb_hft_ordque;
	FuncStraOrdDtlCallback	_cb_hft_orddtl;
	FuncStraTransCallback	_cb_hft_trans;

	FuncEventCallback		_cb_evt;

	FuncParserEvtCallback	_cb_parser_evt;
	FuncParserSubCallback	_cb_parser_sub;

	FuncExecCmdCallback		_cb_exec_cmd;
	FuncExecInitCallback	_cb_exec_init;

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

	CtaStrategyMgr		_cta_mgr;
	HftStrategyMgr		_hft_mgr;
	SelStrategyMgr		_sel_mgr;
	ActionPolicyMgr		_act_policy;

	bool				_is_hft;
	bool				_is_sel;
	bool				_to_exit;

	FuncLoadFnlBars		_ext_fnl_bar_loader;
	FuncLoadRawBars		_ext_raw_bar_loader;
	FuncLoadAdjFactors	_ext_adj_fct_loader;

	void*			_feed_obj;
	FuncReadBars	_feeder_bars;
	FuncReadFactors	_feeder_fcts;
	StdUniqueMutex	_feed_mtx;
};

