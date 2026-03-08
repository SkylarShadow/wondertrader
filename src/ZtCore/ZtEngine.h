/*!
 * \file ZtEngine.h
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
#include "ZtFilterMgr.h"


#include "../Includes/FasterDefs.h"
#include "../Includes/RiskMonDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Share/BoostFile.hpp"
#include "../Share/SpinMutex.hpp"


NS_ZTP_BEGIN
class ZTSSessionInfo;
class ZTSCommodityInfo;
class ZTSContractInfo;

class IBaseDataMgr;
class IHotMgr;

class ZTSVariant;

class ZTSTickData;
struct ZTSBarStruct;
class ZTSTickSlice;
class ZTSKlineSlice;
class ZTSPortFundInfo;

class ZtDtMgr;
class TraderAdapterMgr;

class EventNotifier;

typedef std::function<void()>	TaskItem;

class ZtRiskMonWrapper
{
public:
	ZtRiskMonWrapper(ZtRiskMonitor* mon, IRiskMonitorFact* fact) :_mon(mon), _fact(fact){}
	~ZtRiskMonWrapper()
	{
		if (_mon)
		{
			_fact->deleteRiskMonotor(_mon);
		}
	}

	ZtRiskMonitor* self(){ return _mon; }


private:
	ZtRiskMonitor*		_mon;
	IRiskMonitorFact*	_fact;
};
typedef std::shared_ptr<ZtRiskMonWrapper>	ZtRiskMonPtr;

class IEngineEvtListener
{
public:
	virtual void on_initialize_event() {}
	virtual void on_schedule_event(uint32_t uDate, uint32_t uTime) {}
	virtual void on_session_event(uint32_t uDate, bool isBegin = true) {}
};

class ZtEngine : public ZtPortContext, public IParserStub
{
public:
	ZtEngine();

	inline void set_adapter_mgr(TraderAdapterMgr* mgr) { _adapter_mgr = mgr; }

	void set_date_time(uint32_t curDate, uint32_t curTime, uint32_t curSecs = 0, uint32_t rawTime = 0);

	void set_trading_date(uint32_t curTDate);

	inline uint32_t get_date() { return _cur_date; }
	inline uint32_t get_min_time() { return _cur_time; }
	inline uint32_t get_raw_time() { return _cur_raw_time; }
	inline uint32_t get_secs() { return _cur_secs; }
	inline uint32_t get_trading_date() { return _cur_tdate; }

	inline IBaseDataMgr*		get_basedata_mgr(){ return _base_data_mgr; }
	inline IHotMgr*				get_hot_mgr() { return _hot_mgr; }
	ZTSSessionInfo*		get_session_info(const char* sid, bool isCode = false);
	ZTSCommodityInfo*	get_commodity_info(const char* stdCode);
	ZTSContractInfo*	get_contract_info(const char* stdCode);
	std::string			get_rawcode(const char* stdCode);

	ZTSTickData*	get_last_tick(uint32_t sid, const char* stdCode);
	ZTSTickSlice*	get_tick_slice(uint32_t sid, const char* stdCode, uint32_t count);
	ZTSKlineSlice*	get_kline_slice(uint32_t sid, const char* stdCode, const char* period, uint32_t count, uint32_t times = 1, uint64_t etime = 0);

	void sub_tick(uint32_t sid, const char* code);

	double get_cur_price(const char* stdCode);

	double get_day_price(const char* stdCode, int flag = 0);

	/*
	 *	获取复权因子
	 *	@stdCode	合约代码
	 *	@commInfo	品种信息
	 */
	double get_exright_factor(const char* stdCode, ZTSCommodityInfo* commInfo = NULL);

	uint32_t get_adjusting_flag();

	double calc_fee(const char* stdCode, double price, double qty, uint32_t offset);

	inline void setRiskMonitor(ZtRiskMonPtr& monitor)
	{
		_risk_mon = monitor;
	}

	inline void regEventListener(IEngineEvtListener* listener)
	{
		_evt_listener = listener;
	}

	//////////////////////////////////////////////////////////////////////////
	//ZtPortContext接口
	virtual ZTSPortFundInfo* getFundInfo() override;

	virtual void setVolScale(double scale) override;

	virtual bool isInTrading() override;

	virtual void writeRiskLog(const char* message) override;

	virtual uint32_t	getCurDate() override;
	virtual uint32_t	getCurTime() override;
	virtual uint32_t	getTradingDate() override;
	virtual uint32_t	transTimeToMin(uint32_t uTime) override{ return 0; }

	//////////////////////////////////////////////////////////////////////////
	/// IParserStub接口
	virtual void handle_push_quote(ZTSTickData* newTick) override;

public:
	virtual void init(ZTSVariant* cfg, IBaseDataMgr* bdMgr, ZtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier);

	virtual void run() = 0;

	virtual void on_tick(const char* stdCode, ZTSTickData* curTick);

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, ZTSBarStruct* newBar) = 0;

	virtual void on_init(){}
	virtual void on_session_begin();
	virtual void on_session_end();


protected:
	void		load_fees(const char* filename);

	void		load_datas();

	void		save_datas();

	void		append_signal(const char* stdCode, double qty, bool bStandBy);

	void		do_set_position(const char* stdCode, double qty, double curPx = -1);

	void		task_loop();

	void		push_task(TaskItem task);

	void		update_fund_dynprofit();

	bool		init_riskmon(ZTSVariant* cfg);

private:
	void		init_outputs();
	inline void	log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, double fee = 0.0);
	inline void	log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty,
		double profit, double totalprofit = 0);

protected:
	uint32_t		_cur_date;		//当前日期
	uint32_t		_cur_time;		//当前时间, 是1分钟线时间, 比如0900, 这个时候的1分钟线是0901, _cur_time也就是0901, 这个是为了CTA里面方便
	uint32_t		_cur_raw_time;	//当前真实时间
	uint32_t		_cur_secs;		//当前秒数, 包含毫秒
	uint32_t		_cur_tdate;		//当前交易日

	uint32_t		_fund_udt_span;	//组合资金更新时间间隔

	IBaseDataMgr*	_base_data_mgr;	//基础数据管理器
	IHotMgr*		_hot_mgr;		//主力管理器
	ZtDtMgr*		_data_mgr;		//数据管理器
	IEngineEvtListener*	_evt_listener;

	//By Wesley @ 2022.02.07
	//tick数据订阅项，first是contextid，second是订阅选项，0-原始订阅，1-前复权，2-后复权
	typedef std::pair<uint32_t, uint32_t> SubOpt;
	typedef vvt_hashmap<uint32_t, SubOpt> SubList;
	typedef vvt_hashmap<std::string, SubList>	StraSubMap;
	StraSubMap		_tick_sub_map;	//tick数据订阅表
	StraSubMap		_bar_sub_map;	//K线数据订阅表

	//By Wesley @ 2022.02.07 
	//这个好像没有用到，不需要了
	//vvt_hashset<std::string>		_ticksubed_raw_codes;	//tick订阅表（真实代码模式）
	

	//////////////////////////////////////////////////////////////////////////
	//
	typedef struct _SigInfo
	{
		double		_volume;
		uint64_t	_gentime;

		_SigInfo()
		{
			_volume = 0;
			_gentime = 0;
		}
	}SigInfo;
	typedef vvt_hashmap<std::string, SigInfo>	SignalMap;
	SignalMap		_sig_map;

	//////////////////////////////////////////////////////////////////////////
	//信号过滤器
	ZtFilterMgr		_filter_mgr;
	EventNotifier*	_notifier;

	//////////////////////////////////////////////////////////////////////////
	//手续费模板
	typedef struct _FeeItem
	{
		double	_open;
		double	_close;
		double	_close_today;
		bool	_by_volume;

		_FeeItem()
		{
			memset(this, 0, sizeof(_FeeItem));
		}
	} FeeItem;
	typedef vvt_hashmap<std::string, FeeItem>	FeeMap;
	FeeMap		_fee_map;
	

	ZTSPortFundInfo*	_port_fund;

	//////////////////////////////////////////////////////////////////////////
	//持仓数据
	typedef struct _DetailInfo
	{
		bool		_long;
		double		_price;
		double		_volume;
		uint64_t	_opentime;
		uint32_t	_opentdate;
		double		_profit;

		_DetailInfo()
		{
			memset(this, 0, sizeof(_DetailInfo));
		}
	} DetailInfo;

	typedef struct _PosInfo
	{
		double		_volume;
		double		_closeprofit;
		double		_dynprofit;
		SpinMutex	_mtx;

		std::vector<DetailInfo> _details;

		_PosInfo()
		{
			_volume = 0;
			_closeprofit = 0;
			_dynprofit = 0;
		}
	} PosInfo;
	typedef std::shared_ptr<PosInfo> PosInfoPtr;
	typedef vvt_hashmap<std::string, PosInfoPtr> PositionMap;
	PositionMap		_pos_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef vvt_hashmap<std::string, double> PriceMap;
	PriceMap		_price_map;

	//后台任务线程, 把风控和资金, 持仓更新都放到这个线程里去
	typedef std::queue<TaskItem>	TaskQueue;
	StdThreadPtr	_thrd_task;
	TaskQueue		_task_queue;
	StdUniqueMutex	_mtx_task;
	StdCondVariable	_cond_task;
	bool			_terminated;

	typedef struct _RiskMonFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		IRiskMonitorFact*	_fact;
		FuncCreateRiskMonFact	_creator;
		FuncDeleteRiskMonFact	_remover;
	} RiskMonFactInfo;
	RiskMonFactInfo	_risk_fact;
	ZtRiskMonPtr	_risk_mon;
	double			_risk_volscale;
	uint32_t		_risk_date;

	TraderAdapterMgr*	_adapter_mgr;

	BoostFilePtr	_trade_logs;
	BoostFilePtr	_close_logs;

	vvt_hashmap<std::string, double>	_factors_cache;

	//用于标记是否可以推送tickle
	bool			_ready;
};
NS_ZTP_END