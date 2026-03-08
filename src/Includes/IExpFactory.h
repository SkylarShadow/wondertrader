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
#include "ZTSMarcos.h"

NS_ZTP_BEGIN
class ZTSKlineData;
class ZTSExpressParams;
class ZTSExpressData;
class ZTSHisTrendData;

class IExpFactory
{
public:
	virtual ZTSExpressData *calcKlineExpress(const char* expName, ZTSKlineData* klineData, ZTSExpressParams* params) = 0;
	virtual ZTSExpressData *calcTrendExpress(const char* expName, ZTSHisTrendData* trendData, ZTSExpressParams* params) = 0;
};
NS_ZTP_END