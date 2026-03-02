/*!
 * \file WtHftEngine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include <queue>
#include <functional>
#include <stdint.h>

#include "ParserAdapter.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/RiskMonDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Share/BoostFile.hpp"

#include "../Includes/IUftStraCtx.h"

NS_VVTP_BEGIN
class VvTSSessionInfo;
class VvTSCommodityInfo;
class VvTSContractInfo;

class IBaseDataMgr;
class IHotMgr;

class VvTSVariant;

class VvTSTickData;
struct VvTSBarStruct;
class VvTSTickSlice;
class VvTSKlineSlice;
class VvTSPortFundInfo;

class WtUftDtMgr;
class TraderAdapterMgr;

class EventNotifier;

typedef std::function<void()>	TaskItem;


class VvTSVariant;
class WtUftRtTicker;
class EventNotifier;

typedef std::shared_ptr<IUftStraCtx> UftContextPtr;

class WtUftEngine : public IParserStub
{
public:
	WtUftEngine();
	virtual ~WtUftEngine();

public:
	inline void set_adapter_mgr(TraderAdapterMgr* mgr) { _adapter_mgr = mgr; }

	void set_date_time(uint32_t curDate, uint32_t curTime, uint32_t curSecs = 0, uint32_t rawTime = 0);

	void set_trading_date(uint32_t curTDate);

	inline uint32_t get_date() { return _cur_date; }
	inline uint32_t get_min_time() { return _cur_time; }
	inline uint32_t get_raw_time() { return _cur_raw_time; }
	inline uint32_t get_secs() { return _cur_secs; }
	inline uint32_t get_trading_date() { return _cur_tdate; }

	inline IBaseDataMgr*		get_basedata_mgr() { return _base_data_mgr; }
	VvTSSessionInfo*		get_session_info(const char* sid, bool isCode = false);
	VvTSCommodityInfo*	get_commodity_info(const char* stdCode);
	VvTSContractInfo*	get_contract_info(const char* stdCode);

	VvTSTickData*	get_last_tick(uint32_t sid, const char* stdCode);
	VvTSTickSlice*	get_tick_slice(uint32_t sid, const char* stdCode, uint32_t count);
	VvTSKlineSlice*	get_kline_slice(uint32_t sid, const char* stdCode, const char* period, uint32_t count, uint32_t times = 1, uint64_t etime = 0);

	void sub_tick(uint32_t sid, const char* code);

	double get_cur_price(const char* stdCode);

	void notify_params_update(const char* name);

public:
	void init(VvTSVariant* cfg, IBaseDataMgr* bdMgr, WtUftDtMgr* dataMgr, EventNotifier* notifier);

	void run();

	void on_tick(const char* stdCode, VvTSTickData* curTick);

	void on_bar(const char* stdCode, const char* period, uint32_t times, VvTSBarStruct* newBar);

	void on_init(){}

	void on_session_begin();

	void on_session_end();

	virtual void handle_push_quote(VvTSTickData* newTick) override;
	virtual void handle_push_order_detail(VvTSOrdDtlData* curOrdDtl) override;
	virtual void handle_push_order_queue(VvTSOrdQueData* curOrdQue) override;
	virtual void handle_push_transaction(VvTSTransData* curTrans) override;

public:
	VvTSOrdQueSlice* get_order_queue_slice(uint32_t sid, const char* stdCode, uint32_t count);
	VvTSOrdDtlSlice* get_order_detail_slice(uint32_t sid, const char* stdCode, uint32_t count);
	VvTSTransSlice* get_transaction_slice(uint32_t sid, const char* stdCode, uint32_t count);

public:
	void on_minute_end(uint32_t curDate, uint32_t curTime);

	void addContext(UftContextPtr ctx);

	UftContextPtr	getContext(uint32_t id);

	void sub_order_queue(uint32_t sid, const char* stdCode);
	void sub_order_detail(uint32_t sid, const char* stdCode);
	void sub_transaction(uint32_t sid, const char* stdCode);

private:
	uint32_t		_cur_date;	//当前日期
	uint32_t		_cur_time;		//当前时间, 是1分钟线时间, 比如0900, 这个时候的1分钟线是0901, _cur_time也就是0901, 这个是为了CTA里面方便
	uint32_t		_cur_raw_time;	//当前真实时间
	uint32_t		_cur_secs;	//当前秒数, 包含毫秒
	uint32_t		_cur_tdate;	//当前交易日

	IBaseDataMgr*	_base_data_mgr;	//基础数据管理器
	WtUftDtMgr*		_data_mgr;		//数据管理器

	//By Wesley @ 2022.02.07
	//tick数据订阅项，first是contextid，second是订阅选项，0-原始订阅，1-前复权，2-后复权
	typedef vvt_hashset<uint32_t> SubList;
	typedef vvt_hashmap<std::string, SubList>	StraSubMap;
	StraSubMap		_tick_sub_map;	//tick数据订阅表
	StraSubMap		_ordque_sub_map;	//委托队列订阅表
	StraSubMap		_orddtl_sub_map;	//委托明细订阅表
	StraSubMap		_trans_sub_map;		//成交明细订阅表
	StraSubMap		_bar_sub_map;	//K线数据订阅表	

	TraderAdapterMgr*	_adapter_mgr;

	typedef vvt_hashmap<uint32_t, UftContextPtr> ContextMap;
	ContextMap		_ctx_map;

	WtUftRtTicker*	_tm_ticker;
	VvTSVariant*		_cfg;

	bool			_dependent;	//子策略独立记账

	EventNotifier*	_notifier;
};

NS_VVTP_END