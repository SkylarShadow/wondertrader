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
#include "../Includes/VvTSTypes.h"

/*
*	回调函数定义
*/
typedef void(PORTER_FLAG *FuncOnAccount)(const char* channelid, VvTUInt32 curTDate, const char* currency, double prebalance, double balance, 
	double dynbalance, double closeprofit, double dynprofit, double fee, double margin, double deposit, double withdraw, bool isLast);

typedef void(PORTER_FLAG *FuncOnOrder)(const char* channelid, const char* exchg, const char* code, VvTUInt32 curTDate, 
	const char* orderid, VvTUInt32 direct, VvTUInt32 offset, double volume, double leftover, double traded, double price, VvTUInt32 ordertype, 
	VvTUInt32 pricetype, VvTUInt64 ordertime, VvTUInt32 state, const char* statemsg, bool isLast);

typedef void(PORTER_FLAG *FuncOnTrade)(const char* channelid, const char* exchg, const char* code, VvTUInt32 curTDate,  
	const char* tradeid, const char* orderid, VvTUInt32 direct, VvTUInt32 offset, double volume, double price, 
	double amount, VvTUInt32 ordertype, VvTUInt32 tradetype, VvTUInt64 tradetime, bool isLast);

typedef void(PORTER_FLAG *FuncOnPosition)(const char* channelid, const char* exchg, const char* code, VvTUInt32 curTDate, VvTUInt32 direct,
	double volume, double cost, double margin, double avgpx, double dynprofit, VvTUInt32 volscale, bool isLast);

