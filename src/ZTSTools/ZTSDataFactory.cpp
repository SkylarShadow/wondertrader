/*!
 * \file ZTSDataFactory.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ZTSDataFactory.h"
#include "../Includes/ZTSDataDef.hpp"
#include "../Includes/ZTSContractInfo.hpp"
#include "../Includes/ZTSSessionInfo.hpp"
#include "../Share/TimeUtils.hpp"

using namespace std;


ZTSBarStruct* ZTSDataFactory::updateKlineData(ZTSKlineData* klineData, ZTSTickData* tick, ZTSSessionInfo* sInfo, bool bAlignSec/* = false*/)
{
	if(klineData == NULL || tick == NULL)
		return NULL;

	if(strcmp(klineData->code(), tick->code()) != 0)
		return NULL;

	if(sInfo == NULL)
		return NULL;

	if (!sInfo->isInTradingTime(tick->actiontime() / 100000))
		return NULL;

	ZTSKlinePeriod period = klineData->period();
	switch( period )
	{
	case KP_Tick:
		return updateSecData(sInfo, klineData, tick);
		break;
	case KP_Minute1:
		return updateMin1Data(sInfo, klineData, tick, bAlignSec);
	case KP_Minute5:
		return updateMin5Data(sInfo, klineData, tick, bAlignSec);
	case KP_DAY:
		return updateDayData(sInfo, klineData, tick);
	default:
		return NULL;
	}
}

ZTSBarStruct* ZTSDataFactory::updateKlineData(ZTSKlineData* klineData, ZTSBarStruct* newBasicBar, ZTSSessionInfo* sInfo, bool bAlignSec/* = false*/)
{
	if (klineData == NULL || newBasicBar == NULL)
		return NULL;

	if (sInfo == NULL)
		return NULL;

	ZTSKlinePeriod period = klineData->period();
	switch (period)
	{
	case KP_Minute1:
		return updateMin1Data(sInfo, klineData, newBasicBar, bAlignSec);
	case KP_Minute5:
		return updateMin5Data(sInfo, klineData, newBasicBar, bAlignSec);
	default:
		return NULL;
	}
}

ZTSBarStruct* ZTSDataFactory::updateMin1Data(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSBarStruct* newBasicBar, bool bAlignSec/* = false*/)
{
	if (sInfo == NULL)
		return NULL;

	auto secMins = sInfo->getSecMinList();

	if(klineData->times() == 1)
	{
		klineData->appendBar(*newBasicBar);
		klineData->setClosed(true);
		return klineData->at(-1);
	}

	//计算时间步长
	uint32_t steplen = klineData->times();

	const ZTSBarStruct& curBar = *newBasicBar;

	uint32_t uTradingDate = curBar.date;
	uint32_t uDate = TimeUtils::minBarToDate(curBar.time);
	if (uDate == 19900000)
		uDate = uTradingDate;
	uint32_t uTime = TimeUtils::minBarToTime(curBar.time);
	uint32_t uMinute = sInfo->timeToMinutes(uTime);
	uint32_t uBarMin = 0;

	/*
	 *	By Wesley @ 2023.05.31
	 *	这里是按小节对齐的核心逻辑
	 *	1、先增加一个基础分钟数，如果不按小节对齐，就固定为0
	 *	2、如果按小节对齐，则判断当前分钟处于哪个小节，然后以上个小节结束的分钟数做基础分钟数
	 *	3、然后根据基础分钟数的差量计算新的对齐分钟数
	 *	4、最终得到bar的时间戳
	 */
	if (bAlignSec)
	{
		auto it = std::lower_bound(secMins.begin(), secMins.end(), uMinute);
		auto secIdx = it - secMins.begin();
		if (secIdx == 0)
		{
			uMinute -= 1;
			uBarMin = (uMinute / steplen)*steplen + steplen;
			if (uBarMin > secMins[secIdx])
				uBarMin = secMins[secIdx];
		}
		else
		{
			uMinute -= secMins[secIdx - 1];
			uBarMin = secMins[secIdx - 1] + (uMinute / steplen)*steplen + steplen;
			if (uBarMin > secMins[secIdx])
				uBarMin = secMins[secIdx];
		}
	}
	else
	{
		uMinute -= 1;
		uBarMin = (uMinute / steplen)*steplen + steplen;
	}

	uint64_t uBarTime = sInfo->minuteToTime(uBarMin);
	if (uBarTime < uTime)
		uDate = TimeUtils::getNextDate(uDate, 1);
	uBarTime = TimeUtils::timeToMinBar(uDate, (uint32_t)uBarTime);

	ZTSBarStruct* lastBar = NULL;
	if (klineData->size() > 0)
	{
		lastBar = klineData->at(klineData->size() - 1);
	}

	bool bNewBar = false;
	if (lastBar == NULL || lastBar->date != uDate || lastBar->time != uBarTime)
	{
		//只要日期和时间都不符,则认为已经是一条新的bar了
		lastBar = new ZTSBarStruct();
		bNewBar = true;

		memcpy(lastBar, &curBar, sizeof(ZTSBarStruct));
		lastBar->date = uDate;
		lastBar->time = uBarTime;
	}
	else
	{
		bNewBar = false;

		lastBar->high = max(lastBar->high, curBar.high);
		lastBar->low = min(lastBar->low, curBar.low);
		lastBar->close = curBar.close;
		lastBar->settle = curBar.settle;

		lastBar->vol += curBar.vol;
		lastBar->money += curBar.money;
		lastBar->add += curBar.add;
		lastBar->hold = curBar.hold;
	}

	if(lastBar->time > curBar.time)
	{
		klineData->setClosed(false);
	}
	else
	{
		klineData->setClosed(true);
	}

	if (bNewBar)
	{
		klineData->appendBar(*lastBar);
		delete lastBar;

		return klineData->at(-1);
	}

	return NULL;
}

ZTSBarStruct* ZTSDataFactory::updateMin1Data(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSTickData* tick, bool bAlignSec /* = false */)
{
	//uint32_t curTime = tick->actiontime()/100000;

	uint32_t steplen = klineData->times();

	auto secMins = sInfo->getSecMinList();

	uint32_t uDate = tick->actiondate();
	uint32_t uTime = tick->actiontime() / 100000;
	uint32_t uMinute = sInfo->timeToMinutes(uTime);
	if(uMinute == INVALID_UINT32)
	{
		if(tick->volume() != 0)
		{
			ZTSBarStruct *bar = klineData->at(klineData->size()-1);
			bar->close = tick->price();
			bar->high = max(bar->high,tick->price());
			bar->low = min(bar->low,tick->price());
			bar->vol += tick->volume();
			bar->money += tick->turnover();
			bar->hold = tick->openinterest();
			bar->add += tick->additional();
		}

		return NULL;
	}

	if (sInfo->isLastOfSection(uTime))
	{
		uMinute--;
	}

	uint32_t uBarMin = 0;

	/*
	 *	By Wesley @ 2023.05.31
	 *	这里是按小节对齐的核心逻辑
	 *	1、先增加一个基础分钟数，如果不按小节对齐，就固定为0
	 *	2、如果按小节对齐，则判断当前分钟处于哪个小节，然后以上个小节结束的分钟数做基础分钟数
	 *	3、然后根据基础分钟数的差量计算新的对齐分钟数
	 *	4、最终得到bar的时间戳
	 */
	if (bAlignSec)
	{
		auto it = std::lower_bound(secMins.begin(), secMins.end(), uMinute);
		auto secIdx = it - secMins.begin();
		if (secIdx == 0)
		{
			uBarMin = (uMinute / steplen)*steplen + steplen;
			if (uBarMin > secMins[secIdx])
				uBarMin = secMins[secIdx];
		}
		else
		{
			uMinute -= secMins[secIdx - 1];
			uBarMin = secMins[secIdx - 1] + (uMinute / steplen)*steplen + steplen;
			if (uBarMin > secMins[secIdx])
				uBarMin = secMins[secIdx];
		}
	}
	else
	{
		uBarMin = (uMinute / steplen)*steplen + steplen;
	}

	uint32_t uOnlyMin = sInfo->minuteToTime(uBarMin);
	if(uOnlyMin == 0)
	{
		uDate = TimeUtils::getNextDate(uDate);
	}
	uint64_t uBarTime = TimeUtils::timeToMinBar(uDate, uOnlyMin);

	uint64_t lastTime = klineData->time(-1);
	uint32_t lastDate = klineData->date(-1);
	if (lastTime == INVALID_UINT32 || uBarTime > lastTime || tick->tradingdate() > lastDate)
	{
		//如果时间不一致,则新增一条K线
		ZTSBarStruct *day = new ZTSBarStruct;
		day->date = tick->tradingdate();
		day->time = uBarTime;
		day->open = tick->price();
		day->high = tick->price();
		day->low = tick->price();
		day->close = tick->price();
		day->vol = tick->volume();
		day->money = tick->turnover();
		day->hold = tick->openinterest();
		day->add = tick->additional();

		klineData->appendBar(*day);
		delete day;

		return klineData->at(-1);
	}
	else if (lastTime != INVALID_UINT32 && uBarTime < lastTime)
	{
		//这种情况主要为了防止日期反复出现
		return NULL;
	}
	else
	{
		ZTSBarStruct *bar = klineData->at(klineData->size()-1);
		bar->close = tick->price();
		bar->high = max(bar->high,tick->price());
		bar->low = min(bar->low,tick->price());
		bar->vol += tick->volume();
		bar->money += tick->turnover();
		bar->hold = tick->openinterest();
		bar->add += tick->additional();

		return NULL;
	}
}

ZTSBarStruct* ZTSDataFactory::updateMin5Data(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSBarStruct* newBasicBar, bool bAlignSec/* = false*/)
{
	if (sInfo == NULL)
		return NULL;

	auto secMins = sInfo->getSecMinList();

	if (klineData->times() == 1)
	{
		klineData->appendBar(*newBasicBar);
		return klineData->at(-1);
	}

	//计算时间步长
	uint32_t steplen = 5 * klineData->times();

	const ZTSBarStruct& curBar = *newBasicBar;

	uint32_t uTradingDate = curBar.date;
	uint32_t uDate = TimeUtils::minBarToDate(curBar.time);
	if (uDate == 19900000)
		uDate = uTradingDate;
	uint32_t uTime = TimeUtils::minBarToTime(curBar.time);
	uint32_t uMinute = sInfo->timeToMinutes(uTime);
	uint32_t uBarMin = 0;
	/*
	 *	By Wesley @ 2023.05.31
	 *	这里是按小节对齐的核心逻辑
	 *	1、先增加一个基础分钟数，如果不按小节对齐，就固定为0
	 *	2、如果按小节对齐，则判断当前分钟处于哪个小节，然后以上个小节结束的分钟数做基础分钟数
	 *	3、然后根据基础分钟数的差量计算新的对齐分钟数
	 *	4、最终得到bar的时间戳
	 */
	if (bAlignSec)
	{
		auto it = std::lower_bound(secMins.begin(), secMins.end(), uMinute);
		auto secIdx = it - secMins.begin();
		if (secIdx == 0)
		{
			uMinute -= 5;
			uBarMin = (uMinute / steplen)*steplen + steplen;
			if (uBarMin > secMins[secIdx])
				uBarMin = secMins[secIdx];
		}
		else
		{
			uMinute -= secMins[secIdx - 1];
			uBarMin = secMins[secIdx - 1] + (uMinute / steplen)*steplen + steplen;
			if (uBarMin > secMins[secIdx])
				uBarMin = secMins[secIdx];
		}
	}
	else
	{
		uMinute -= 5;
		uBarMin = (uMinute / steplen)*steplen + steplen;
	}

	uint64_t uBarTime = sInfo->minuteToTime(uBarMin);
	if (uBarTime < uTime)
		uDate = TimeUtils::getNextDate(uDate, 1);
	uBarTime = TimeUtils::timeToMinBar(uDate, (uint32_t)uBarTime);

	ZTSBarStruct* lastBar = NULL;
	if (klineData->size() > 0)
	{
		lastBar = klineData->at(klineData->size() - 1);
	}

	bool bNewBar = false;
	if (lastBar == NULL || lastBar->date != uDate || lastBar->time != uBarTime)
	{

		//只要日期和时间都不符,则认为已经是一条新的bar了
		lastBar = new ZTSBarStruct();
		bNewBar = true;

		memcpy(lastBar, &curBar, sizeof(ZTSBarStruct));
		lastBar->date = uTradingDate;
		lastBar->time = uBarTime;
	}
	else
	{
		bNewBar = false;

		lastBar->high = max(lastBar->high, curBar.high);
		lastBar->low = min(lastBar->low, curBar.low);
		lastBar->close = curBar.close;
		lastBar->settle = curBar.settle;

		lastBar->vol += curBar.vol;
		lastBar->money += curBar.money;
		lastBar->add += curBar.add;
		lastBar->hold = curBar.hold;
	}

	if (lastBar->time > curBar.time)
	{
		klineData->setClosed(false);
	}
	else
	{
		klineData->setClosed(true);
	}

	if (bNewBar)
	{
		klineData->appendBar(*lastBar);
		delete lastBar;

		return klineData->at(-1);
	}

	return NULL;
}

ZTSBarStruct* ZTSDataFactory::updateMin5Data(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSTickData* tick, bool bAlignSec /* = false */)
{
	auto secMins = sInfo->getSecMinList();

	uint32_t steplen = 5*klineData->times();

	uint32_t uDate = tick->actiondate();
	uint32_t uTime = tick->actiontime()/100000;
	uint32_t uMinute = sInfo->timeToMinutes(uTime);
	if (sInfo->isLastOfSection(uTime))
	{
		uMinute--;
	}

	uint32_t uBarMin = 0;
	/*
	 *	By Wesley @ 2023.05.31
	 *	这里是按小节对齐的核心逻辑
	 *	1、先增加一个基础分钟数，如果不按小节对齐，就固定为0
	 *	2、如果按小节对齐，则判断当前分钟处于哪个小节，然后以上个小节结束的分钟数做基础分钟数
	 *	3、然后根据基础分钟数的差量计算新的对齐分钟数
	 *	4、最终得到bar的时间戳
	 */
	if (bAlignSec)
	{
		auto it = std::lower_bound(secMins.begin(), secMins.end(), uMinute);
		auto secIdx = it - secMins.begin();
		if (secIdx == 0)
		{
			uBarMin = (uMinute / steplen)*steplen + steplen;
			if (uBarMin > secMins[secIdx])
				uBarMin = secMins[secIdx];
		}
		else
		{
			uMinute -= secMins[secIdx - 1];
			uBarMin = secMins[secIdx - 1] + (uMinute / steplen)*steplen + steplen;
			if (uBarMin > secMins[secIdx])
				uBarMin = secMins[secIdx];
		}
	}
	else
	{
		uBarMin = (uMinute / steplen)*steplen + steplen;
	}

	uint32_t uOnlyMin = sInfo->minuteToTime(uBarMin);
	if (uOnlyMin == 0)
	{
		uDate = TimeUtils::getNextDate(uDate);
	}
	uint64_t uBarTime = TimeUtils::timeToMinBar(uDate, uOnlyMin);

	uint64_t lastTime = klineData->time(klineData->size()-1);
	if(lastTime == INVALID_UINT32 || uBarTime != lastTime)
	{
		//如果时间不一致,则新增一条K线
		ZTSBarStruct *day = new ZTSBarStruct;
		day->date = tick->tradingdate();
		day->time = uBarTime;
		day->open = tick->price();
		day->high = tick->price();
		day->low = tick->price();
		day->close = tick->price();
		day->vol = tick->volume();
		day->money = tick->turnover();
		day->hold = tick->openinterest();
		day->add = tick->additional();

		klineData->appendBar(*day);
		delete day;

		return klineData->at(-1);
	}
	else
	{
		ZTSBarStruct *bar = klineData->at(klineData->size()-1);
		bar->close = tick->price();
		bar->high = max(bar->high,tick->price());
		bar->low = min(bar->low,tick->price());
		bar->vol += tick->volume();
		bar->money += tick->turnover();
		bar->hold = tick->openinterest();
		bar->add = tick->additional();

		return NULL;
	}
}

ZTSBarStruct* ZTSDataFactory::updateDayData(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSTickData* tick)
{
	uint32_t curDate = tick->tradingdate();
	uint32_t lastDate = klineData->date(klineData->size()-1);

	if(lastDate == INVALID_UINT32 || curDate != lastDate)
	{
		//如果时间不一致,则新增一条K线
		ZTSBarStruct *day = new ZTSBarStruct;
		day->date = curDate;
		day->time = 0;
		day->open = tick->price();
		day->high = tick->price();
		day->low = tick->price();
		day->close = tick->price();
		day->vol = tick->volume();
		day->money = tick->turnover();
		day->hold = tick->openinterest();
		day->add = tick->additional();

		return day;
	}
	else
	{
		ZTSBarStruct *bar = klineData->at(klineData->size()-1);
		bar->close = tick->price();
		bar->high = max(bar->high,tick->price());
		bar->low = min(bar->low,tick->price());
		bar->vol += tick->volume();
		bar->money += tick->turnover();
		bar->hold = tick->openinterest();
		bar->add += tick->additional();

		return NULL;
	}
}

ZTSBarStruct* ZTSDataFactory::updateSecData(ZTSSessionInfo* sInfo, ZTSKlineData* klineData, ZTSTickData* tick)
{
	uint32_t seconds = klineData->times();
	uint32_t curSeconds = sInfo->timeToSeconds(tick->actiontime()/1000);
	uint32_t barSeconds = (curSeconds/seconds)*seconds + seconds;
	uint32_t barTime = sInfo->secondsToTime(barSeconds);

	if(klineData->isUnixTime())
	{
		uint32_t uDate = tick->actiondate();
		if (barTime < tick->actiontime() / 1000)
			uDate = TimeUtils::getNextDate(uDate);
		barTime = (uint32_t)(TimeUtils::makeTime(uDate, barTime * 1000) / 1000);
	}	

	uint64_t lastTime = klineData->time(klineData->size()-1);
	if(lastTime == INVALID_UINT32 || lastTime != barTime)
	{
		ZTSBarStruct *day = new ZTSBarStruct;
		day->date = tick->tradingdate();
		day->time = barTime;
		day->open = tick->price();
		day->high = tick->price();
		day->low = tick->price();
		day->close = tick->price();
		day->vol = tick->volume();
		day->money = tick->turnover();
		day->hold = tick->openinterest();
		day->add = tick->additional();

		return day;
	}
	else
	{
		ZTSBarStruct *bar = klineData->at(klineData->size()-1);
		bar->close = tick->price();
		bar->high = max(bar->high,tick->price());
		bar->low = min(bar->low,tick->price());
		bar->vol += tick->volume();
		bar->money += tick->turnover();
		bar->hold = tick->openinterest();
		bar->add += tick->additional();

		return NULL;
	}
}

uint32_t ZTSDataFactory::getPrevMinute(uint32_t curMinute, int period /* = 1 */)
{
	uint32_t h = curMinute/100;
	uint32_t m = curMinute%100;
	if(m == 0)
	{
		m = 60;
		if(h == 0) h = 24;

		return (h-1)*100 + (m-period);
	}
	else
	{
		return h*100 + m - period;
	}
}

ZTSKlineData* ZTSDataFactory::extractKlineData(ZTSKlineSlice* baseKline, ZTSKlinePeriod period, uint32_t times, ZTSSessionInfo* sInfo, 
		bool bIncludeOpen /* = true */, bool bAlignSec /* = false */)
{
	if(baseKline == NULL || baseKline->size() == 0)
		return NULL;

	//一倍,则不需要转换
	if(times <= 1 || period == KP_Tick)
	{
		return NULL;
	}

	if(period == KP_DAY)
	{
		return extractDayData(baseKline, times, bIncludeOpen);
	}
	else if(period == KP_Minute1)
	{
		return extractMin1Data(baseKline, times, sInfo, bIncludeOpen, bAlignSec);
	}
	else if(period == KP_Minute5)
	{
		return extractMin5Data(baseKline, times, sInfo, bIncludeOpen, bAlignSec);
	}
	
	return NULL;
}

ZTSKlineData* ZTSDataFactory::extractMin1Data(ZTSKlineSlice* baseKline, uint32_t times, ZTSSessionInfo* sInfo, bool bIncludeOpen /* = true */, bool bAlignSec /* = false */)
{
	//根据合约代码获取市场信息
	if(sInfo == NULL)
		return NULL;

	//计算时间步长
	uint32_t steplen = times;

	/*
	 *	By Wesley @ 2023.05.31
	 *	要增加一个按照小节对齐的重采样方式
	 *	一般逻辑就是每个小节开始重新计算条数，然后在小节结束时，强制对齐
	 */
	auto secMins = sInfo->getSecMinList();

	ZTSKlineData* ret = ZTSKlineData::create(baseKline->code(), 0);
	ret->setPeriod(KP_Minute1, times);

	for (auto i = 0; i < baseKline->size(); i++)
	{
		const ZTSBarStruct& curBar = *baseKline->at(i);

		uint32_t uTradingDate = curBar.date;
		uint32_t uDate = TimeUtils::minBarToDate(curBar.time);
		if(uDate == 19900000)
			uDate = uTradingDate;
		uint32_t uTime = TimeUtils::minBarToTime(curBar.time);
		uint32_t uMinute = sInfo->timeToMinutes(uTime);
		uint32_t uBarMin = 0;

		/*
		 *	By Wesley @ 2023.05.31
		 *	这里是按小节对齐的核心逻辑
		 *	1、先增加一个基础分钟数，如果不按小节对齐，就固定为0
		 *	2、如果按小节对齐，则判断当前分钟处于哪个小节，然后以上个小节结束的分钟数做基础分钟数
		 *	3、然后根据基础分钟数的差量计算新的对齐分钟数
		 *	4、最终得到bar的时间戳
		 */
		if(bAlignSec)
		{
			auto it = std::lower_bound(secMins.begin(), secMins.end(), uMinute);
			auto secIdx = it - secMins.begin();
			if(secIdx == 0)
			{
				uMinute -= 1;
				uBarMin = (uMinute / steplen)*steplen + steplen;
				if (uBarMin > secMins[secIdx])
					uBarMin = secMins[secIdx];
			}
			else
			{
				uMinute -= secMins[secIdx - 1];
				uBarMin = secMins[secIdx - 1] + (uMinute / steplen)*steplen + steplen;
				if (uBarMin > secMins[secIdx])
					uBarMin = secMins[secIdx];
			}
		}
		else
		{
			uMinute -= 1;
			uBarMin = (uMinute / steplen)*steplen + steplen;
		}

		uint64_t uBarTime = sInfo->minuteToTime(uBarMin);
		if (uBarTime < uTime)
			uDate = TimeUtils::getNextDate(uDate, 1);
		uBarTime = TimeUtils::timeToMinBar(uDate, (uint32_t)uBarTime);

		ZTSBarStruct* lastBar = NULL;
		if(ret->size() > 0)
		{
			lastBar = ret->at(ret->size()-1);
		}

		bool bNewBar = false;
		if(lastBar == NULL || lastBar->time != uBarTime)
		{
			//只要日期和时间都不符,则认为已经是一条新的bar了
			lastBar = new ZTSBarStruct();
			bNewBar = true;

			memcpy(lastBar, &curBar, sizeof(ZTSBarStruct));
			lastBar->date = uDate;
			lastBar->time = uBarTime;
		}
		else
		{
			bNewBar = false;

			lastBar->high = max(lastBar->high, curBar.high);
			lastBar->low = min(lastBar->low, curBar.low);
			lastBar->close = curBar.close;
			lastBar->settle = curBar.settle;

			lastBar->vol += curBar.vol;
			lastBar->money += curBar.money;
			lastBar->add += curBar.add;
			lastBar->hold = curBar.hold;
		}

		if(bNewBar)
		{
			ret->appendBar(*lastBar);
			delete lastBar;
		}
	}

	//检查最后一条数据
	{
		ZTSBarStruct* lastRawBar = baseKline->at(-1);
		ZTSBarStruct* lastDesBar = ret->at(-1);
		//如果目标K线的最后一条数据的日期或者时间大于原始K线最后一条的日期或时间
		if ( lastDesBar->date > lastRawBar->date || lastDesBar->time > lastRawBar->time)
		{
			if (!bIncludeOpen)
				ret->getDataRef().resize(ret->size() - 1);
			else
				ret->setClosed(false);
		}
	}
	

	return ret;
}

ZTSKlineData* ZTSDataFactory::extractMin5Data(ZTSKlineSlice* baseKline, uint32_t times, ZTSSessionInfo* sInfo, bool bIncludeOpen /* = true */, bool bAlignSec /* = false */)
{
	if(sInfo == NULL)
		return NULL;

	//计算时间步长
	uint32_t steplen = 5*times;
	/*
	 *	By Wesley @ 2023.05.31
	 *	要增加一个按照小节对齐的重采样方式
	 *	一般逻辑就是每个小节开始重新计算条数，然后在小节结束时，强制对齐
	 */
	auto secMins = sInfo->getSecMinList();

	ZTSKlineData* ret = ZTSKlineData::create(baseKline->code(), 0);
	ret->setPeriod(KP_Minute5, times);

	for (auto i = 0; i < baseKline->size(); i++)
	{
		const ZTSBarStruct& curBar = *baseKline->at(i);

		uint32_t uTradingDate = curBar.date;
		uint32_t uDate = TimeUtils::minBarToDate(curBar.time);
		if(uDate == 19900000)
			uDate = uTradingDate;
		uint32_t uTime = TimeUtils::minBarToTime(curBar.time);
		uint32_t uMinute = sInfo->timeToMinutes(uTime);
		uint32_t uBarMin = 0;
		/*
		 *	By Wesley @ 2023.05.31
		 *	这里是按小节对齐的核心逻辑
		 *	1、先增加一个基础分钟数，如果不按小节对齐，就固定为0
		 *	2、如果按小节对齐，则判断当前分钟处于哪个小节，然后以上个小节结束的分钟数做基础分钟数
		 *	3、然后根据基础分钟数的差量计算新的对齐分钟数
		 *	4、最终得到bar的时间戳
		 */
		if (bAlignSec)
		{
			auto it = std::lower_bound(secMins.begin(), secMins.end(), uMinute);
			auto secIdx = it - secMins.begin();
			if (secIdx == 0)
			{
				uMinute -= 5;
				uBarMin = (uMinute / steplen)*steplen + steplen;
				if (uBarMin > secMins[secIdx])
					uBarMin = secMins[secIdx];
			}
			else
			{
				uMinute -= secMins[secIdx - 1];
				uBarMin = secMins[secIdx - 1] + (uMinute / steplen)*steplen + steplen;
				if (uBarMin > secMins[secIdx])
					uBarMin = secMins[secIdx];
			}
		}
		else
		{
			uMinute -= 5;
			uBarMin = (uMinute / steplen)*steplen + steplen;
		}

		uint64_t uBarTime = sInfo->minuteToTime(uBarMin);
		if (uBarTime < uTime)
			uDate = TimeUtils::getNextDate(uDate, 1);
		uBarTime = TimeUtils::timeToMinBar(uDate, (uint32_t)uBarTime);

		ZTSBarStruct* lastBar = NULL;
		if(ret->size() > 0)
		{
			lastBar = ret->at(ret->size()-1);
		}

		bool bNewBar = false;
		if(lastBar == NULL || lastBar->time != uBarTime)
		{
			//只要日期和时间都不符,则认为已经是一条新的bar了
			lastBar = new ZTSBarStruct();
			bNewBar = true;

			memcpy(lastBar, &curBar, sizeof(ZTSBarStruct));
			lastBar->date = uTradingDate;
			lastBar->time = uBarTime;
		}
		else
		{
			bNewBar = false;

			lastBar->high = max(lastBar->high, curBar.high);
			lastBar->low = min(lastBar->low, curBar.low);
			lastBar->close = curBar.close;
			lastBar->settle = curBar.settle;

			lastBar->vol += curBar.vol;
			lastBar->money += curBar.money;
			lastBar->add += curBar.add;
			lastBar->hold = curBar.hold;
		}

		if(bNewBar)
		{
			ret->appendBar(*lastBar);
			delete lastBar;
		}
	}

	//检查最后一条数据
	{
		ZTSBarStruct* lastRawBar = baseKline->at(-1);
		ZTSBarStruct* lastDesBar = ret->at(-1);
		//如果目标K线的最后一条数据的日期或者时间大于原始K线最后一条的日期或时间
		if (lastDesBar->date > lastRawBar->date || lastDesBar->time > lastRawBar->time)
		{
			if (!bIncludeOpen)
				ret->getDataRef().resize(ret->size() - 1);
			else
				ret->setClosed(false);
		}
	}

	return ret;
}

ZTSKlineData* ZTSDataFactory::extractDayData(ZTSKlineSlice* baseKline, uint32_t times, bool bIncludeOpen /* = true */)
{
	//计算时间步长
	uint32_t steplen = times;

	ZTSKlineData* ret = ZTSKlineData::create(baseKline->code(), 0);
	ret->setPeriod(KP_DAY, times);

	uint32_t count = 0;
	//ZTSKlineData::ZTSBarList& bars = baseKline->getDataRef();
	//ZTSKlineData::ZTSBarList::const_iterator it = bars.begin();
	//for(; it != bars.end(); it++,count++)
	for (auto i = 0; i < baseKline->size(); i++, count++)
	{
		const ZTSBarStruct& curBar = *baseKline->at(i);

		uint32_t uDate = curBar.date;

		ZTSBarStruct* lastBar = NULL;
		if(ret->size() > 0)
		{
			lastBar = ret->at(ret->size()-1);
		}

		bool bNewBar = false;
		if(lastBar == NULL || count == steplen)
		{
			//只要日期和时间都不符,则认为已经是一条新的bar了
			lastBar = new ZTSBarStruct();
			bNewBar = true;

			memcpy(lastBar, &curBar, sizeof(ZTSBarStruct));
			lastBar->date = uDate;
			lastBar->time = 0;
			count = 0;
		}
		else
		{
			bNewBar = false;

			lastBar->high = max(lastBar->high, curBar.high);
			lastBar->low = min(lastBar->low, curBar.low);
			lastBar->close = curBar.close;
			lastBar->settle = curBar.settle;

			lastBar->vol += curBar.vol;
			lastBar->money += curBar.money;
			lastBar->add = curBar.add;
			lastBar->hold = curBar.hold;
		}

		if(bNewBar)
		{
			ret->appendBar(*lastBar);
			delete lastBar;
		}
	}

	return ret;
}

ZTSKlineData* ZTSDataFactory::extractKlineData(ZTSTickSlice* ayTicks, uint32_t seconds, 
	ZTSSessionInfo* sInfo, bool bUnixTime /* = false */, bool bAlignSec /* = false */)
{
	if(ayTicks == NULL || ayTicks->size() == 0)
		return NULL;
	
	const ZTSTickStruct& firstTick = *(ayTicks->at(0));

	if(sInfo == NULL)
		return NULL;

	ZTSKlineData* ret = ZTSKlineData::create(firstTick.code,0);
	ret->setPeriod(KP_Tick, seconds);
	ret->setUnixTime(bUnixTime);

	for (uint32_t i = 0; i < ayTicks->size(); i++)
	{
		ZTSBarStruct* lastBar = NULL;
		if(ret->size() > 0)
		{
			lastBar = ret->at(ret->size()-1);
		}

		const ZTSTickStruct* curTick = ayTicks->at(i);
		uint32_t uDate = curTick->trading_date;
		uint32_t curSeconds = sInfo->timeToSeconds(curTick->action_time/1000);
		uint32_t barSeconds = (curSeconds/seconds)*seconds + seconds;
		uint64_t barTime = sInfo->secondsToTime(barSeconds);

		//如果计算出来的K线时间戳小于tick数据的时间戳
		uint32_t actDt = curTick->action_date;
		if (barTime < curTick->action_time / 1000)
		{
			actDt = TimeUtils::getNextDate(actDt);
		}

		if(bUnixTime)
		{
			barTime = (uint64_t)TimeUtils::makeTime(actDt, (long)(barTime * 1000)) / 1000;
		}
		else
		{
			barTime = (uint64_t)actDt * 1000000 + barTime;
		}

		bool bNewBar = false;
		if (lastBar == NULL || uDate != lastBar->date || barTime != lastBar->time)
		{
			lastBar = new ZTSBarStruct();
			bNewBar = true;

			lastBar->date = uDate;
			lastBar->time = barTime;

			lastBar->open = curTick->price;
			lastBar->high = curTick->price;
			lastBar->low = curTick->price;
			lastBar->close = curTick->price;
			lastBar->vol = curTick->volume;
			lastBar->money = curTick->turn_over;
			lastBar->hold = curTick->open_interest;
			lastBar->add = curTick->diff_interest;
		}
		else
		{
			lastBar->close = curTick->price;
			lastBar->high = max(lastBar->high,curTick->price);
			lastBar->low = min(lastBar->low,curTick->price);
			lastBar->vol += curTick->volume;
			lastBar->money += curTick->turn_over;
			lastBar->hold = curTick->open_interest;
			lastBar->add += curTick->diff_interest;
		}

		if(bNewBar)
		{
			ret->appendBar(*lastBar);
			delete lastBar;
		}
	}

	return ret;
}

bool ZTSDataFactory::mergeKlineData(ZTSKlineData* klineData, ZTSKlineData* newKline)
{
	if (klineData == NULL || newKline == NULL)
		return false;

	if (strcmp(klineData->code(), newKline->code()) != 0)
		return false;

	if (!(klineData->period() == newKline->period() && klineData->times() == newKline->times()))
		return false;

	ZTSKlineData::ZTSBarList& bars = klineData->getDataRef();
	ZTSKlineData::ZTSBarList& newBars = newKline->getDataRef();
	if(bars.empty())
	{
		bars.swap(newBars);
		newBars.clear();
		return true;
	}
	else
	{
		uint64_t sTime,eTime;
		if(klineData->period() == KP_DAY)
		{
			sTime = bars[0].date;
			eTime = bars[bars.size() - 1].date;
		}
		else
		{
			sTime = bars[0].time;
			eTime = bars[bars.size() - 1].time;
		}

		ZTSKlineData::ZTSBarList tempHead, tempTail;
		uint32_t count = newKline->size();
		for (uint32_t i = 0; i < count; i++)
		{
			ZTSBarStruct& curBar = newBars[i];

			uint64_t curTime;
			if (klineData->period() == KP_DAY)
				curTime = curBar.date;
			else
				curTime = curBar.time;

			if(curTime < sTime)
			{
				tempHead.emplace_back(curBar);
			}
			else if(curTime > eTime)
			{
				tempTail.emplace_back(curBar);
			}
		}

		bars.insert(bars.begin(), tempHead.begin(), tempHead.end());
		bars.insert(bars.end(), tempTail.begin(), tempTail.end());
	}
	
	return true;
}
