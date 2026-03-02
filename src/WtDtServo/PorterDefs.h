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

NS_VVTP_BEGIN
struct WTSBarStruct;
struct WTSTickStruct;
NS_VVTP_END

USING_NS_VVTP;

typedef void(PORTER_FLAG *FuncGetBarsCallback)(WTSBarStruct* bar, VvtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(WTSTickStruct* tick, VvtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncCountDataCallback)(VvtUInt32 dataCnt);
typedef void(PORTER_FLAG *FuncOnTickCallback)(const char* stdCode, WTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncOnBarCallback)(const char* stdCode, const char* period, WTSBarStruct* bar);

