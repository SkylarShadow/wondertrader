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
#include "../Includes/ZTSTypes.h"

/*
*	回调函数定义
*/
typedef void(PORTER_FLAG *FuncOnAccount)(const char* channelid, ZTUInt32 curTDate, const char* currency, double prebalance, double balance, 
	double dynbalance, double closeprofit, double dynprofit, double fee, double margin, double deposit, double withdraw, bool isLast);

typedef void(PORTER_FLAG *FuncOnOrder)(const char* channelid, const char* exchg, const char* code, ZTUInt32 curTDate, 
	const char* orderid, ZTUInt32 direct, ZTUInt32 offset, double volume, double leftover, double traded, double price, ZTUInt32 ordertype, 
	ZTUInt32 pricetype, ZTUInt64 ordertime, ZTUInt32 state, const char* statemsg, bool isLast);

typedef void(PORTER_FLAG *FuncOnTrade)(const char* channelid, const char* exchg, const char* code, ZTUInt32 curTDate,  
	const char* tradeid, const char* orderid, ZTUInt32 direct, ZTUInt32 offset, double volume, double price, 
	double amount, ZTUInt32 ordertype, ZTUInt32 tradetype, ZTUInt64 tradetime, bool isLast);

typedef void(PORTER_FLAG *FuncOnPosition)(const char* channelid, const char* exchg, const char* code, ZTUInt32 curTDate, ZTUInt32 direct,
	double volume, double cost, double margin, double avgpx, double dynprofit, ZTUInt32 volscale, bool isLast);

