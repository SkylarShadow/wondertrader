#pragma once
#include "../VvtBtCore/SelMocker.h"

class ExpSelMocker : public SelMocker
{
public:
	ExpSelMocker(HisDataReplayer* replayer, const char* name, int32_t slippage = 0, bool isRatioSlp = false);
	virtual ~ExpSelMocker();

public:
	virtual void on_init() override;

	virtual void on_session_begin(uint32_t uDate) override;

	virtual void on_session_end(uint32_t uDate) override;

	virtual void on_bactest_end() override;

	virtual void on_tick_updated(const char* stdCode, VvTSTickData* newTick) override;

	virtual void on_bar_close(const char* stdCode, const char* period, VvTSBarStruct* newBar) override;

	virtual void on_strategy_schedule(uint32_t curDate, uint32_t curTime) override;
};

