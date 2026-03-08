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
struct ZTSTickStruct;
struct ZTSBarStruct;
struct ZTSOrdDtlStruct;
struct ZTSOrdQueStruct;
struct ZTSTransStruct;
NS_ZTP_END

USING_NS_ZTP;

//////////////////////////////////////////////////////////////////////////
//扩展Parser回调函数
static const ZTUInt32	EVENT_PARSER_INIT = 1;	//Parser初始化
static const ZTUInt32	EVENT_PARSER_CONNECT = 2;	//Parser连接
static const ZTUInt32	EVENT_PARSER_DISCONNECT = 3;	//Parser断开连接
static const ZTUInt32	EVENT_PARSER_RELEASE = 4;	//Parser释放

typedef void(PORTER_FLAG *FuncParserEvtCallback)(ZTUInt32 evtId, const char* id);
typedef void(PORTER_FLAG *FuncParserSubCallback)(const char* id, const char* fullCode, bool isForSub);


//////////////////////////////////////////////////////////////////////////
//扩展Dumper回调函数
typedef bool(PORTER_FLAG *FuncDumpBars)(const char* id, const char* stdCode, const char* period, ZTSBarStruct* bars, ZTUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpTicks)(const char* id, const char* stdCode, ZTUInt32 uDate, ZTSTickStruct* ticks, ZTUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpOrdQue)(const char* id, const char* stdCode, ZTUInt32 uDate, ZTSOrdQueStruct* items, ZTUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpOrdDtl)(const char* id, const char* stdCode, ZTUInt32 uDate, ZTSOrdDtlStruct* items, ZTUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpTrans)(const char* id, const char* stdCode, ZTUInt32 uDate, ZTSTransStruct* items, ZTUInt32 count);
