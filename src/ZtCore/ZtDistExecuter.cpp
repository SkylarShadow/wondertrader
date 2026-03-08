#include "ZtDistExecuter.h"

#include "../Includes/ZTSVariant.hpp"

#include "../Share/decimal.h"
#include "../ZTSTools/ZTSLogger.h"

USING_NS_ZTP;

ZtDistExecuter::ZtDistExecuter(const char* name)
	: IExecCommand(name)
{

}

ZtDistExecuter::~ZtDistExecuter()
{

}

bool ZtDistExecuter::init(ZTSVariant* params)
{
	if (params == NULL)
		return false;

	_config = params;
	_config->retain();

	_scale = params->getUInt32("scale");

	return true;
}

void ZtDistExecuter::set_position(const vvt_hashmap<std::string, double>& targets)
{
	for (auto it = targets.begin(); it != targets.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		double newVol = it->second;

		newVol *= _scale;
		double oldVol = _target_pos[stdCode];
		_target_pos[stdCode] = newVol;
		if (!decimal::eq(oldVol, newVol))
		{
			ZTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "[{}]{}目标仓位更新: {} -> {}", _name.c_str(), stdCode, oldVol, newVol);
		}

		//这里广播目标仓位
	}
}

void ZtDistExecuter::on_position_changed(const char* stdCode, double targetPos)
{
	targetPos *= _scale;

	double oldVol = _target_pos[stdCode];
	_target_pos[stdCode] = targetPos;

	if (!decimal::eq(oldVol, targetPos))
	{
		ZTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "[{}]{}目标仓位更新: {} -> {}", _name.c_str(), stdCode, oldVol, targetPos);
	}

	//这里广播目标仓位
}

void ZtDistExecuter::on_tick(const char* stdCode, ZTSTickData* newTick)
{
	//分布式执行器不需要处理ontick
}
