/*!
 * \file ZtCtaEngine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../Includes/ICtaStraCtx.h"
#include "../Share/threadpool.hpp"
#include "ZtExecMgr.h"
#include "ZtEngine.h"

NS_ZTP_BEGIN
class ZTSVariant;
typedef std::shared_ptr<ICtaStraCtx> CtaContextPtr;

class ZtCtaRtTicker;

class ZtCtaEngine : public ZtEngine, public IExecuterStub
{
public:
	ZtCtaEngine();
	virtual ~ZtCtaEngine();

public:
	//////////////////////////////////////////////////////////////////////////
	//ZtEngine接口
	virtual void handle_push_quote(ZTSTickData* newTick) override;

	virtual void on_tick(const char* stdCode, ZTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, ZTSBarStruct* newBar) override;

	virtual void on_init() override;
	virtual void on_session_begin() override;
	virtual void on_session_end() override;

	virtual void run() override;

	virtual void init(ZTSVariant* cfg, IBaseDataMgr* bdMgr, ZtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier) override;

	virtual bool isInTrading() override;
	virtual uint32_t transTimeToMin(uint32_t uTime) override;

	///////////////////////////////////////////////////////////////////////////
	//IExecuterStub 接口
	virtual uint64_t get_real_time() override;
	virtual ZTSCommodityInfo* get_comm_info(const char* stdCode) override;
	virtual ZTSSessionInfo* get_sess_info(const char* stdCode) override;
	virtual IHotMgr* get_hot_mon() { return _hot_mgr; }
	virtual uint32_t get_trading_day() { return _cur_tdate; }


public:
	void on_schedule(uint32_t curDate, uint32_t curTime);	

	void handle_pos_change(const char* straName, const char* stdCode, double diffPos);

	void addContext(CtaContextPtr ctx);
	
	CtaContextPtr	getContext(uint32_t id);

	inline void addExecuter(ExecCmdPtr executer)
	{
		_exec_mgr.add_executer(executer);
		executer->setStub(this);
	}

	inline bool loadRouterRules(ZTSVariant* cfg)
	{
		return _exec_mgr.load_router_rules(cfg);
	}

public:
	void notify_chart_marker(uint64_t time, const char* straId, double price, const char* icon, const char* tag);
	void notify_chart_index(uint64_t time, const char* straId, const char* idxName, const char* lineName, double val);
	void notify_trade(const char* straId, const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, const char* userTag);

private:
	typedef vvt_hashmap<uint32_t, CtaContextPtr> ContextMap;
	ContextMap		_ctx_map;

	ZtCtaRtTicker*	_tm_ticker;

	ZtExecuterMgr	_exec_mgr;

	ZTSVariant*		_cfg;

	typedef std::shared_ptr<boost::threadpool::pool> ThreadPoolPtr;
	ThreadPoolPtr		_pool;
};

NS_ZTP_END

