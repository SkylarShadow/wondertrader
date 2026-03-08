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

NS_ZTP_BEGIN
struct ZTSBarStruct;
struct ZTSTickStruct;
NS_ZTP_END

USING_NS_ZTP;

typedef void(PORTER_FLAG *FuncGetBarsCallback)(ZTSBarStruct* bar, ZTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(ZTSTickStruct* tick, ZTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncCountDataCallback)(ZTUInt32 dataCnt);
typedef void(PORTER_FLAG *FuncOnTickCallback)(const char* stdCode, ZTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncOnBarCallback)(const char* stdCode, const char* period, ZTSBarStruct* bar);

