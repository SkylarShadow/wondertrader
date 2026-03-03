/*!
 * \file CtaStraContext.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "CtaStraBaseCtx.h"

#include "../Includes/VvTSDataDef.hpp"

NS_VVTP_BEGIN
class VvtCtaEngine;
NS_VVTP_END

USING_NS_VVTP;

class CtaStrategy;

class CtaStraContext : public CtaStraBaseCtx
{
public:
	CtaStraContext(VvtCtaEngine* engine, const char* name, int32_t slippage);
	virtual ~CtaStraContext();

	void set_strategy(CtaStrategy* stra){ _strategy = stra; }
	CtaStrategy* get_stragety() { return _strategy; }

public:
	//»Øµ÷º¯Êý
	virtual void on_init() override;
	virtual void on_session_begin(uint32_t uTDate) override;
	virtual void on_session_end(uint32_t uTDate) override;
	virtual void on_tick_updated(const char* stdCode, VvTSTickData* newTick) override;
	virtual void on_bar_close(const char* stdCode, const char* period, VvTSBarStruct* newBar) override;
	virtual void on_calculate(uint32_t curDate, uint32_t curTime) override;
	virtual void on_condition_triggered(const char* stdCode, double target, double price, const char* usertag) override;

private:
	CtaStrategy*		_strategy;
};


