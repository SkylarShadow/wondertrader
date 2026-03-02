#pragma once
#include "IExecCommand.h"

NS_VVTP_BEGIN
class VVTSVariant;

class WtDistExecuter : public IExecCommand
{
public:
	WtDistExecuter(const char* name);
	virtual ~WtDistExecuter();

public:
	/*
	 *	初始化执行器
	 *	传入初始化参数
	 */
	bool init(VVTSVariant* params);


public:
	//////////////////////////////////////////////////////////////////////////
	//IExecCommand
	virtual void set_position(const vvt_hashmap<std::string, double>& targets) override;


	virtual void on_position_changed(const char* stdCode, double targetPos) override;


	virtual void on_tick(const char* stdCode, WTSTickData* newTick) override;

private:
	VVTSVariant*			_config;

	uint32_t			_scale;

	vvt_hashmap<std::string, double> _target_pos;
};
NS_VVTP_END

