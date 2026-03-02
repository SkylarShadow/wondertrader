/*!
 * \file HftStrategyDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>
#include <stdint.h>

#include "../Includes/VvTSMarcos.h"

NS_VVTP_BEGIN
class VvTSVariant;
class IHftStraCtx;
class VvTSTickData;
class VvTSOrdDtlData;
class VvTSOrdQueData;
class VvTSTransData;
struct VvTSBarStruct;
NS_VVTP_END

USING_NS_VVTP;

class HftStrategy
{
public:
	HftStrategy(const char* id) :_id(id){}
	virtual ~HftStrategy(){}

public:
	/*
	 *	执行单元名称
	 */
	virtual const char* getName() = 0;

	/*
	 *	所属执行器工厂名称
	 */
	virtual const char* getFactName() = 0;

	/*
	 *	初始化
	 */
	virtual bool init(VvTSVariant* cfg){ return true; }

	virtual const char* id() const { return _id.c_str(); }

	//回调函数

	/*
	 *	策略初始化
	 */
	virtual void on_init(IHftStraCtx* ctx) = 0;

	/*
	 *	交易日开始
	 */
	virtual void on_session_begin(IHftStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	交易日结束
	 */
	virtual void on_session_end(IHftStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	tick回调
	 */
	virtual void on_tick(IHftStraCtx* ctx, const char* code, VvTSTickData* newTick) {}

	/*
	 *	委托队列回调
	 */
	virtual void on_order_queue(IHftStraCtx* ctx, const char* code, VvTSOrdQueData* newOrdQue) {}

	/*
	 *	委托明细回调
	 */
	virtual void on_order_detail (IHftStraCtx* ctx, const char* code, VvTSOrdDtlData* newOrdDtl) {}

	/*
	 *	逐笔成交回调
	 */
	virtual void on_transaction(IHftStraCtx* ctx, const char* code, VvTSTransData* newTrans) {}

	/*
	 *	K线闭合回调
	 */
	virtual void on_bar(IHftStraCtx* ctx, const char* code, const char* period, uint32_t times, VvTSBarStruct* newBar) {}

	/*
	 *	成交回报
	 */
	virtual void on_trade(IHftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag) {}

	/*
	 *	持仓回报
	 *	只有在刚启动的时候，交易接口就绪之前会触发该回调
	 */
	virtual void on_position(IHftStraCtx* ctx, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail) {}

	/*
	 *	订单回报
	 */
	virtual void on_order(IHftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag) {}

	/*
	 *	交易通道就绪回调
	 */
	virtual void on_channel_ready(IHftStraCtx* ctx) {}

	/*
	 *	交易通道断开回调
	 */
	virtual void on_channel_lost(IHftStraCtx* ctx) {}

	/*
	 *	委托回报
	 */
	virtual void on_entrust(uint32_t localid, bool bSuccess, const char* message, const char* userTag) {}

protected:
	std::string _id;
};

//////////////////////////////////////////////////////////////////////////
//策略工厂接口
typedef void(*FuncEnumHftStrategyCallback)(const char* factName, const char* straName, bool isLast);

class IHftStrategyFact
{
public:
	IHftStrategyFact(){}
	virtual ~IHftStrategyFact(){}

public:
	/*
	 *	获取工厂名
	 */
	virtual const char* getName() = 0;

	/*
	 *	枚举策略
	 */
	virtual void enumStrategy(FuncEnumHftStrategyCallback cb) = 0;

	/*
	 *	根据名称创建执行单元
	 */
	virtual HftStrategy* createStrategy(const char* name, const char* id) = 0;

	/*
	 *	删除执行单元
	 */
	virtual bool deleteStrategy(HftStrategy* stra) = 0;
};

//创建执行工厂
typedef IHftStrategyFact* (*FuncCreateHftStraFact)();
//删除执行工厂
typedef void(*FuncDeleteHftStraFact)(IHftStrategyFact* &fact);
