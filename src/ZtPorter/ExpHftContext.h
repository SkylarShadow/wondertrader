#pragma once
#include "../ZtCore/HftStraBaseCtx.h"

USING_NS_ZTP;

class ExpHftContext : public HftStraBaseCtx
{
public:
	ExpHftContext(ZtHftEngine* engine, const char* name, bool bAgent, int32_t slippage):HftStraBaseCtx(engine, name, bAgent, slippage){}
	virtual ~ExpHftContext(){}

public:
	virtual void on_bar(const char* code, const char* period, uint32_t times, ZTSBarStruct* newBar) override;

	virtual void on_channel_lost() override;

	virtual void on_channel_ready() override;

	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	virtual void on_init() override;

	virtual void on_session_begin(uint32_t uTDate) override;

	virtual void on_session_end(uint32_t uTDate) override;

	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled) override;

	virtual void on_tick(const char* code, ZTSTickData* newTick) override;

	virtual void on_order_queue(const char* stdCode, ZTSOrdQueData* newOrdQue) override;

	virtual void on_order_detail(const char* stdCode, ZTSOrdDtlData* newOrdDtl) override;

	virtual void on_transaction(const char* stdCode, ZTSTransData* newTrans) override;

	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) override;

	virtual void on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday) override;
};

