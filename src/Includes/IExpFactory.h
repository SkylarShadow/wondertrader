/*!
 * \file IExpFactory.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 指标工接口定义
 */
#pragma once
#include "VvTSMarcos.h"

NS_VVTP_BEGIN
class VvTSKlineData;
class VvTSExpressParams;
class VvTSExpressData;
class VvTSHisTrendData;

class IExpFactory
{
public:
	virtual VvTSExpressData *calcKlineExpress(const char* expName, VvTSKlineData* klineData, VvTSExpressParams* params) = 0;
	virtual VvTSExpressData *calcTrendExpress(const char* expName, VvTSHisTrendData* trendData, VvTSExpressParams* params) = 0;
};
NS_VVTP_END