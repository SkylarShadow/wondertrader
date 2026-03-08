/*!
 * \file ZTSDataFactory.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据拼接工厂类定义
 */
#pragma once
#include "../Includes/IDataFactory.h"

USING_NS_ZTP;

class ZTSDataFactory : public IDataFactory
{
public:
	/*
	 *	利用tick数据更新K线
	 *	@klineData	K线数据
	 *	@tick		tick数据
	 *	@sInfo		交易时间模板
	 */
	virtual ZTSBarStruct*	updateKlineData(ZTSKlineData* klineData, ZTSTickData* tick, ZTSSessionInfo* sInfo, bool bAlignSec = false);

	/*
	 *	利用基础周期K线数据更新K线
	 *	@klineData		K线数据
	 *	@newBasicBar	基础周期K线数据
	 *	@sInfo			交易时间模板
	 */
	virtual ZTSBarStruct*	updateKlineData(ZTSKlineData* klineData, ZTSBarStruct* newBasicBar, ZTSSessionInfo* sInfo, bool bAlignSec = false);

	/*
	 *	从基础周期K线数据提取非基础周期的K线数据
	 *	@baseKline	基础周期K线
	 *	@period		基础周期，m1/m5/day
	 *	@times		周期倍数
	 *	@sInfo		交易时间模板
	 *	@bIncludeOpen	是否包含未闭合的K线
	 *	@bAlignSec	是否按小节对齐
	 */
	virtual ZTSKlineData*	extractKlineData(ZTSKlineSlice* baseKline, ZTSKlinePeriod period, uint32_t times, ZTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);

	/*
	 *	从tick数据提取秒周期的K线数据
	 *	@ayTicks	tick数据
	 *	@seconds	目标周期
	 *	@sInfo		交易时间模板
	 *	@bUnixTime	tick时间戳是否是unixtime
	 *	@bAlignSec	是否按小节对齐
	 */
	virtual ZTSKlineData*	extractKlineData(ZTSTickSlice* ayTicks, uint32_t seconds, ZTSSessionInfo* sInfo, bool bUnixTime = false, bool bAlignSec = false);

	/*
	 *	合并K线
	 *	@klineData	目标K线
	 *	@newKline	待合并的K线
	 */
	virtual bool			mergeKlineData(ZTSKlineData* klineData, ZTSKlineData* newKline);

protected:
	ZTSBarStruct* updateMin1Data(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSTickData* tick, bool bAlignSec = false);
	ZTSBarStruct* updateMin5Data(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSTickData* tick, bool bAlignSec = false);
	ZTSBarStruct* updateDayData(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSTickData* tick);
	ZTSBarStruct* updateSecData(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSTickData* tick);

	ZTSBarStruct* updateMin1Data(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSBarStruct* newBasicBar, bool bAlignSec = false);
	ZTSBarStruct* updateMin5Data(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSBarStruct* newBasicBar, bool bAlignSec = false);

	ZTSKlineData* extractMin1Data(ZTSKlineSlice* baseKline, uint32_t times, ZTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);
	ZTSKlineData* extractMin5Data(ZTSKlineSlice* baseKline, uint32_t times, ZTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);
	ZTSKlineData* extractDayData(ZTSKlineSlice* baseKline, uint32_t times, bool bIncludeOpen = true);

protected:
	static uint32_t getPrevMinute(uint32_t curMinute, int period = 1);
};

