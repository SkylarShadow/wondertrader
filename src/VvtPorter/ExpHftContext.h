#pragma once
#include "../VvtCore/HftStraBaseCtx.h"

USING_NS_VVTP;

class ExpHftContext : public HftStraBaseCtx
{
public:
	ExpHftContext(VvtHftEngine* engine, const char* name, bool bAgent, int32_t slippage):HftStraBaseCtx(engine, name, bAgent, slippage){}
	virtual ~ExpHftContext(){}

public:
	virtual void on_bar(const char* code, const char* period, uint32_t times, VvTSBarStruct* newBar) override;

	virtual void on_channel_lost() override;

	virtual void on_channel_ready() override;

	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	virtual void on_init() override;

	virtual void on_session_begin(uint32_t uTDate) override;

	virtual void on_session_end(uint32_t uTDate) override;

	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled) override;

	virtual void on_tick(const char* code, VvTSTickData* newTick) override;

	virtual void on_order_queue(const char* stdCode, VvTSOrdQueData* newOrdQue) override;

	virtual void on_order_detail(const char* stdCode, VvTSOrdDtlData* newOrdDtl) override;

	virtual void on_transaction(const char* stdCode, VvTSTransData* newTrans) override;

	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) override;

	virtual void on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday) override;
};

