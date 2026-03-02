/*!
 * \file PorterDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include <stdint.h>
#include "../Includes/WTSTypes.h"

/*
*	回调函数定义
*/
typedef void(PORTER_FLAG *FuncOnAccount)(const char* channelid, VvtUInt32 curTDate, const char* currency, double prebalance, double balance, 
	double dynbalance, double closeprofit, double dynprofit, double fee, double margin, double deposit, double withdraw, bool isLast);

typedef void(PORTER_FLAG *FuncOnOrder)(const char* channelid, const char* exchg, const char* code, VvtUInt32 curTDate, 
	const char* orderid, VvtUInt32 direct, VvtUInt32 offset, double volume, double leftover, double traded, double price, VvtUInt32 ordertype, 
	VvtUInt32 pricetype, VvtUInt64 ordertime, VvtUInt32 state, const char* statemsg, bool isLast);

typedef void(PORTER_FLAG *FuncOnTrade)(const char* channelid, const char* exchg, const char* code, VvtUInt32 curTDate,  
	const char* tradeid, const char* orderid, VvtUInt32 direct, VvtUInt32 offset, double volume, double price, 
	double amount, VvtUInt32 ordertype, VvtUInt32 tradetype, VvtUInt64 tradetime, bool isLast);

typedef void(PORTER_FLAG *FuncOnPosition)(const char* channelid, const char* exchg, const char* code, VvtUInt32 curTDate, VvtUInt32 direct,
	double volume, double cost, double margin, double avgpx, double dynprofit, VvtUInt32 volscale, bool isLast);

