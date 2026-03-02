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

NS_VVTP_BEGIN
struct VvTSBarStruct;
struct VvTSTickStruct;
NS_VVTP_END

USING_NS_VVTP;

typedef void(PORTER_FLAG *FuncGetBarsCallback)(VvTSBarStruct* bar, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(VvTSTickStruct* tick, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncCountDataCallback)(VvTUInt32 dataCnt);
typedef void(PORTER_FLAG *FuncOnTickCallback)(const char* stdCode, VvTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncOnBarCallback)(const char* stdCode, const char* period, VvTSBarStruct* bar);

