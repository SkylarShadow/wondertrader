/*!
 * \file VvTSDataFactory.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据拼接工厂类定义
 */
#pragma once
#include "../Includes/IDataFactory.h"

USING_NS_VVTP;

class VvTSDataFactory : public IDataFactory
{
public:
	/*
	 *	利用tick数据更新K线
	 *	@klineData	K线数据
	 *	@tick		tick数据
	 *	@sInfo		交易时间模板
	 */
	virtual VvTSBarStruct*	updateKlineData(VvTSKlineData* klineData, VvTSTickData* tick, VvTSSessionInfo* sInfo, bool bAlignSec = false);

	/*
	 *	利用基础周期K线数据更新K线
	 *	@klineData		K线数据
	 *	@newBasicBar	基础周期K线数据
	 *	@sInfo			交易时间模板
	 */
	virtual VvTSBarStruct*	updateKlineData(VvTSKlineData* klineData, VvTSBarStruct* newBasicBar, VvTSSessionInfo* sInfo, bool bAlignSec = false);

	/*
	 *	从基础周期K线数据提取非基础周期的K线数据
	 *	@baseKline	基础周期K线
	 *	@period		基础周期，m1/m5/day
	 *	@times		周期倍数
	 *	@sInfo		交易时间模板
	 *	@bIncludeOpen	是否包含未闭合的K线
	 *	@bAlignSec	是否按小节对齐
	 */
	virtual VvTSKlineData*	extractKlineData(VvTSKlineSlice* baseKline, VvTSKlinePeriod period, uint32_t times, VvTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);

	/*
	 *	从tick数据提取秒周期的K线数据
	 *	@ayTicks	tick数据
	 *	@seconds	目标周期
	 *	@sInfo		交易时间模板
	 *	@bUnixTime	tick时间戳是否是unixtime
	 *	@bAlignSec	是否按小节对齐
	 */
	virtual VvTSKlineData*	extractKlineData(VvTSTickSlice* ayTicks, uint32_t seconds, VvTSSessionInfo* sInfo, bool bUnixTime = false, bool bAlignSec = false);

	/*
	 *	合并K线
	 *	@klineData	目标K线
	 *	@newKline	待合并的K线
	 */
	virtual bool			mergeKlineData(VvTSKlineData* klineData, VvTSKlineData* newKline);

protected:
	VvTSBarStruct* updateMin1Data(VvTSSessionInfo* sInfo, VvTSKlineData* klineData, VvTSTickData* tick, bool bAlignSec = false);
	VvTSBarStruct* updateMin5Data(VvTSSessionInfo* sInfo, VvTSKlineData* klineData, VvTSTickData* tick, bool bAlignSec = false);
	VvTSBarStruct* updateDayData(VvTSSessionInfo* sInfo, VvTSKlineData* klineData, VvTSTickData* tick);
	VvTSBarStruct* updateSecData(VvTSSessionInfo* sInfo, VvTSKlineData* klineData, VvTSTickData* tick);

	VvTSBarStruct* updateMin1Data(VvTSSessionInfo* sInfo, VvTSKlineData* klineData, VvTSBarStruct* newBasicBar, bool bAlignSec = false);
	VvTSBarStruct* updateMin5Data(VvTSSessionInfo* sInfo, VvTSKlineData* klineData, VvTSBarStruct* newBasicBar, bool bAlignSec = false);

	VvTSKlineData* extractMin1Data(VvTSKlineSlice* baseKline, uint32_t times, VvTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);
	VvTSKlineData* extractMin5Data(VvTSKlineSlice* baseKline, uint32_t times, VvTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);
	VvTSKlineData* extractDayData(VvTSKlineSlice* baseKline, uint32_t times, bool bIncludeOpen = true);

protected:
	static uint32_t getPrevMinute(uint32_t curMinute, int period = 1);
};

