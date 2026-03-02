/*!
 * \file IDataFactory.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据拼接工厂接口定义
 */
#pragma once
#include <stdint.h>
#include "../Includes/VvTSTypes.h"

//USING_NS_VVTP;

NS_VVTP_BEGIN
class VvTSKlineData;
class VvTSHisTrendData;
class VvTSTickData;
class VvTSSessionInfo;
class VvTSKlineSlice;
class VvTSContractInfo;
struct VvTSBarStruct;
struct VvTSTickStruct;
class VvTSTickSlice;

/*
 *	数据工厂
 *	主要用于各种数据的拼接
 *	这里只定义一个接口
 */
class IDataFactory
{
public:
	/*
	 *	利用tick数据更新K线
	 *	@klineData	K线数据
	 *	@tick		tick数据
	 *	@sInfo		交易时间模板
	 */
	virtual VvTSBarStruct*	updateKlineData(VvTSKlineData* klineData, VvTSTickData* tick, VvTSSessionInfo* sInfo, bool bAlignSec = false)						= 0;

	/*
	 *	利用基础周期K线数据更新K线
	 *	@klineData		K线数据
	 *	@newBasicBar	基础周期K线数据
	 *	@sInfo			交易时间模板
	 */
	virtual VvTSBarStruct*	updateKlineData(VvTSKlineData* klineData, VvTSBarStruct* newBasicBar, VvTSSessionInfo* sInfo, bool bAlignSec = false)				= 0;

	/*
	 *	从基础周期K线数据提取非基础周期的K线数据
	 *	@baseKline	基础周期K线
	 *	@period		基础周期，m1/m5/day
	 *	@times		周期倍数
	 *	@sInfo		交易时间模板
	 *	@bIncludeOpen	是否包含未闭合的K线
	 */
	virtual VvTSKlineData*	extractKlineData(VvTSKlineSlice* baseKline, VvTSKlinePeriod period, uint32_t times, VvTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bSectionSplit = false) = 0;

	/*
	 *	从tick数据提取秒周期的K线数据
	 *	@ayTicks	tick数据
	 *	@seconds	目标周期
	 *	@sInfo		交易时间模板
	 *	@bUnixTime	tick时间戳是否是unixtime
	 */
	virtual VvTSKlineData*	extractKlineData(VvTSTickSlice* ayTicks, uint32_t seconds, VvTSSessionInfo* sInfo, bool bUnixTime = false, bool bSectionSplit = false) = 0;

	/*
	 *	合并K线
	 *	@klineData	目标K线
	 *	@newKline	待合并的K线
	 */
	virtual bool			mergeKlineData(VvTSKlineData* klineData, VvTSKlineData* newKline)											= 0;
};

NS_VVTP_END
