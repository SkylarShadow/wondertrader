#pragma once
#include "../Includes/CtaStrategyDefs.h"

class ZtStraDualThrust : public CtaStrategy
{
public:
	ZtStraDualThrust(const char* id);
	virtual ~ZtStraDualThrust();

public:
	virtual const char* getFactName() override;

	virtual const char* getName() override;

	virtual bool init(ZTSVariant* cfg) override;

	virtual void on_schedule(ICtaStraCtx* ctx, uint32_t curDate, uint32_t curTime) override;

	virtual void on_init(ICtaStraCtx* ctx) override;

	virtual void on_tick(ICtaStraCtx* ctx, const char* stdCode, ZTSTickData* newTick) override;

	virtual void on_session_begin(ICtaStraCtx* ctx, uint32_t uTDate) override;

private:
	//指标参数
	double		_k1;
	double		_k2;
	uint32_t	_days;

	std::string _moncode;

	//数据周期
	std::string _period;
	//K线条数
	uint32_t	_count;

	//合约代码
	std::string _code;

	bool		_isstk;
};

