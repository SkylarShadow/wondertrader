/*!
 * \file TraderAdapter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include "../Includes/ExecuteDefs.h"
#include "../Includes/FasterDefs.h"
#include "../Includes/ITraderApi.h"
#include "../Share/BoostFile.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/SpinMutex.hpp"

NS_VVTP_BEGIN
class VvTSVariant;
class ActionPolicyMgr;
class VvTSContractInfo;
class VvTSCommodityInfo;
class VvtLocalExecuter;
class EventNotifier;

class ITrdNotifySink;

typedef std::function<void(const char*, bool, double, double, double, double)> FuncEnumChnlPosCallBack;

class TraderAdapter : public ITraderSpi
{
public:
	TraderAdapter(EventNotifier* caster = NULL);
	~TraderAdapter();

	typedef enum tagAdapterState
	{
		AS_NOTLOGIN,		//未登录
		AS_LOGINING,		//正在登录
		AS_LOGINED,			//已登录
		AS_LOGINFAILED,		//登录失败
		AS_POSITION_QRYED,	//仓位已查
		AS_ORDERS_QRYED,	//订单已查
		AS_TRADES_QRYED,	//成交已查
		AS_ALLREADY			//全部就绪
	} AdapterState;

	typedef struct _PosItem
	{
		//多仓数据
		double	l_newvol;
		double	l_newavail;
		double	l_prevol;
		double	l_preavail;

		//空仓数据
		double	s_newvol;
		double	s_newavail;
		double	s_prevol;
		double	s_preavail;

		_PosItem()
		{
			memset(this, 0, sizeof(_PosItem));
		}

		double total_pos(bool isLong = true) const
		{
			if (isLong)
				return l_newvol + l_prevol;
			else
				return s_newvol + s_prevol;
		}

		double avail_pos(bool isLong = true) const
		{
			if (isLong)
				return l_newavail + l_preavail;
			else
				return s_newavail + s_preavail;
		}

	} PosItem;

	typedef struct _RiskParams
	{
		uint32_t	_order_times_boundary;
		uint32_t	_order_stat_timespan;
		uint32_t	_order_total_limits;

		uint32_t	_cancel_times_boundary;
		uint32_t	_cancel_stat_timespan;
		uint32_t	_cancel_total_limits;

		_RiskParams()
		{
			memset(this, 0, sizeof(_RiskParams));
		}
	} RiskParams;

public:
	bool init(const char* id, VvTSVariant* params, IBaseDataMgr* bdMgr, ActionPolicyMgr* policyMgr);
	bool initExt(const char* id, ITraderApi* api, IBaseDataMgr* bdMgr, ActionPolicyMgr* policyMgr);

	void release();

	bool run();

	inline const char* id() const{ return _id.c_str(); }

	AdapterState state() const{ return _state; }

	void addSink(ITrdNotifySink* sink)
	{
		_sinks.insert(sink);
	}

	inline bool isReady() const { return _state == AS_ALLREADY; }

	void queryFund();

private:
	uint32_t doEntrust(VvTSEntrust* entrust);
	bool	doCancel(VvTSOrderInfo* ordInfo);

	inline void	printPosition(const char* stdCode, const PosItem& pItem);

	inline VvTSContractInfo* getContract(const char* stdCode);
	inline VvTSCommodityInfo* getCommodify(const char* stdCommID);

	const RiskParams* getRiskParams(const char* stdCode);

	void initSaveData();

	inline void	logTrade(uint32_t localid, const char* stdCode, VvTSTradeInfo* trdInfo);
	inline void	logOrder(uint32_t localid, const char* stdCode, VvTSOrderInfo* ordInfo);

	void	saveData(VvTSArray* ayFunds = NULL);

	inline void updateUndone(const char* stdCode, double qty, bool bOuput = true);

public:
	double getPosition(const char* stdCode, bool bValidOnly, int32_t flag = 3);
	OrderMap* getOrders(const char* stdCode);
	double getUndoneQty(const char* stdCode)
	{
		auto it = _undone_qty.find(stdCode);
		if (it != _undone_qty.end())
			return it->second;

		return 0;
	}

	void enumPosition(FuncEnumChnlPosCallBack cb);

	uint32_t openLong(const char* stdCode, double price, double qty, int flag, VvTSContractInfo* cInfo = NULL);
	uint32_t openShort(const char* stdCode, double price, double qty, int flag, VvTSContractInfo* cInfo = NULL);
	uint32_t closeLong(const char* stdCode, double price, double qty, bool isToday, int flag, VvTSContractInfo* cInfo = NULL);
	uint32_t closeShort(const char* stdCode, double price, double qty, bool isToday, int flag, VvTSContractInfo* cInfo = NULL);
	
	OrderIDs buy(const char* stdCode, double price, double qty, int flag, bool bForceClose, VvTSContractInfo* cInfo = NULL);
	OrderIDs sell(const char* stdCode, double price, double qty, int flag, bool bForceClose, VvTSContractInfo* cInfo = NULL);
	bool	cancel(uint32_t localid);
	OrderIDs cancel(const char* stdCode, bool isBuy, double qty = 0);

	inline bool	isTradeEnabled(const char* stdCode) const;

	bool	checkCancelLimits(const char* stdCode);
	bool	checkOrderLimits(const char* stdCode);

	bool	checkSelfMatch(const char* stdCode, VvTSTradeInfo* tInfo);

	inline	bool isSelfMatched(const char* stdCode)
	{
		//如果忽略自成交，则直接返回false
		if (_ignore_sefmatch)
			return false;

		auto it = _self_matches.find(stdCode);
		return it != _self_matches.end();
	}

public:
	//////////////////////////////////////////////////////////////////////////
	//ITraderSpi接口
	virtual void handleEvent(VvTSTraderEvent e, int32_t ec) override;

	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate) override;

	virtual void onLogout() override;

	virtual void onRspEntrust(VvTSEntrust* entrust, VvTSError *err) override;

	virtual void onRspAccount(VvTSArray* ayAccounts) override;

	virtual void onRspPosition(const VvTSArray* ayPositions) override;

	virtual void onRspOrders(const VvTSArray* ayOrders) override;

	virtual void onRspTrades(const VvTSArray* ayTrades) override;

	virtual void onPushOrder(VvTSOrderInfo* orderInfo) override;

	virtual void onPushTrade(VvTSTradeInfo* tradeRecord) override;

	virtual void onTraderError(VvTSError* err, void* pData = NULL) override;

	virtual IBaseDataMgr* getBaseDataMgr() override;

	virtual void handleTraderLog(VvTSLogLevel ll, const char* message) override;

private:
	VvTSVariant*			_cfg;
	std::string			_id;
	std::string			_order_pattern;

	uint32_t			_trading_day;

	ITraderApi*			_trader_api;
	FuncDeleteTrader	_remover;
	AdapterState		_state;

	EventNotifier*		_notifier;

	vvt_hashset<ITrdNotifySink*>	_sinks;

	IBaseDataMgr*		_bd_mgr;
	ActionPolicyMgr*	_policy_mgr;

	vvt_hashmap<std::string, PosItem> _positions;

	SpinMutex	_mtx_orders;
	OrderMap*	_orders;
	vvt_hashset<std::string> _orderids;	//主要用于标记有没有处理过该订单

	vvt_hashmap<std::string, std::string>		_trade_refs;	//用于记录成交单和订单的匹配
	vvt_hashset<std::string>					_self_matches;	//自成交的合约

	/*
	 *	By Wesley @ 2023.03.16
	 *	加一个控制，这样自成交发生以后，还可以恢复交易
	 */
	bool			_ignore_sefmatch;		//忽略自成交限制

	vvt_hashmap<std::string, double> _undone_qty;	//未完成数量

	typedef VvTSHashMap<std::string>	TradeStatMap;
	TradeStatMap*	_stat_map;	//统计数据

	//这两个缓存时间内的容器,主要是为了控制瞬间流量而设置的
	typedef std::vector<uint64_t> TimeCacheList;
	typedef vvt_hashmap<std::string, TimeCacheList> CodeTimeCacheMap;
	CodeTimeCacheMap	_order_time_cache;	//下单时间缓存
	CodeTimeCacheMap	_cancel_time_cache;	//撤单时间缓存

	//如果被风控了,就会进入到排除队列
	vvt_hashset<std::string>	_exclude_codes;
	
	typedef vvt_hashmap<std::string, RiskParams>	RiskParamsMap;
	RiskParamsMap	_risk_params_map;
	bool			_risk_mon_enabled;

	bool			_save_data;	//是否保存交易日志
	BoostFilePtr	_trades_log;		//交易数据日志
	BoostFilePtr	_orders_log;		//订单数据日志
	std::string		_rt_data_file;		//实时数据文件
};

typedef std::shared_ptr<TraderAdapter>				TraderAdapterPtr;
typedef vvt_hashmap<std::string, TraderAdapterPtr>	TraderAdapterMap;


//////////////////////////////////////////////////////////////////////////
//TraderAdapterMgr
class TraderAdapterMgr : private boost::noncopyable
{
public:
	void	release();

	void	run();

	const TraderAdapterMap& getAdapters() const { return _adapters; }

	TraderAdapterPtr getAdapter(const char* tname);

	bool	addAdapter(const char* tname, TraderAdapterPtr& adapter);

	void	refresh_funds();

private:
	TraderAdapterMap	_adapters;
};

NS_VVTP_END
