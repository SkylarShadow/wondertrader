#pragma once
#include "IExecCommand.h"

NS_ZTP_BEGIN
class ZTSVariant;

class ZtDistExecuter : public IExecCommand
{
public:
	ZtDistExecuter(const char* name);
	virtual ~ZtDistExecuter();

public:
	/*
	 *	初始化执行器
	 *	传入初始化参数
	 */
	bool init(ZTSVariant* params);


public:
	//////////////////////////////////////////////////////////////////////////
	//IExecCommand
	virtual void set_position(const vvt_hashmap<std::string, double>& targets) override;


	virtual void on_position_changed(const char* stdCode, double targetPos) override;


	virtual void on_tick(const char* stdCode, ZTSTickData* newTick) override;

private:
	ZTSVariant*			_config;

	uint32_t			_scale;

	vvt_hashmap<std::string, double> _target_pos;
};
NS_ZTP_END

