/*!
 * \file ZtHftEngine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "ZtEngine.h"
#include "ZtLocalExecuter.h"

#include "../Includes/IHftStraCtx.h"

NS_ZTP_BEGIN

class ZTSVariant;
class ZtHftRtTicker;

typedef std::shared_ptr<IHftStraCtx> HftContextPtr;

class ZtHftEngine :	public ZtEngine
{
public:
	ZtHftEngine();
	virtual ~ZtHftEngine();

public:
	//////////////////////////////////////////////////////////////////////////
	//ZtEngine 接口
	virtual void init(ZTSVariant* cfg, IBaseDataMgr* bdMgr, ZtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier) override;

	virtual void run() override;

	virtual void handle_push_quote(ZTSTickData* newTick) override;
	virtual void handle_push_order_detail(ZTSOrdDtlData* curOrdDtl) override;
	virtual void handle_push_order_queue(ZTSOrdQueData* curOrdQue) override;
	virtual void handle_push_transaction(ZTSTransData* curTrans) override;

	virtual void on_tick(const char* stdCode, ZTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, ZTSBarStruct* newBar) override;

	virtual void on_session_begin() override;

	virtual void on_session_end() override;

public:
	ZTSOrdQueSlice* get_order_queue_slice(uint32_t sid, const char* stdCode, uint32_t count);
	ZTSOrdDtlSlice* get_order_detail_slice(uint32_t sid, const char* stdCode, uint32_t count);
	ZTSTransSlice* get_transaction_slice(uint32_t sid, const char* stdCode, uint32_t count);

public:
	void on_minute_end(uint32_t curDate, uint32_t curTime);

	void addContext(HftContextPtr ctx);

	HftContextPtr	getContext(uint32_t id);

	void sub_order_queue(uint32_t sid, const char* stdCode);
	void sub_order_detail(uint32_t sid, const char* stdCode);
	void sub_transaction(uint32_t sid, const char* stdCode);

private:
	typedef vvt_hashmap<uint32_t, HftContextPtr> ContextMap;
	ContextMap		_ctx_map;

	ZtHftRtTicker*	_tm_ticker;
	ZTSVariant*		_cfg;


	StraSubMap		_ordque_sub_map;	//委托队列订阅表
	StraSubMap		_orddtl_sub_map;	//委托明细订阅表
	StraSubMap		_trans_sub_map;		//成交明细订阅表
};

NS_ZTP_END