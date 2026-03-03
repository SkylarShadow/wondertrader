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
struct VvTSTickStruct;
struct VvTSBarStruct;
struct VvTSOrdDtlStruct;
struct VvTSOrdQueStruct;
struct VvTSTransStruct;
NS_VVTP_END

USING_NS_VVTP;

//////////////////////////////////////////////////////////////////////////
//扩展Parser回调函数
static const VvTUInt32	EVENT_PARSER_INIT = 1;	//Parser初始化
static const VvTUInt32	EVENT_PARSER_CONNECT = 2;	//Parser连接
static const VvTUInt32	EVENT_PARSER_DISCONNECT = 3;	//Parser断开连接
static const VvTUInt32	EVENT_PARSER_RELEASE = 4;	//Parser释放

typedef void(PORTER_FLAG *FuncParserEvtCallback)(VvTUInt32 evtId, const char* id);
typedef void(PORTER_FLAG *FuncParserSubCallback)(const char* id, const char* fullCode, bool isForSub);


//////////////////////////////////////////////////////////////////////////
//扩展Dumper回调函数
typedef bool(PORTER_FLAG *FuncDumpBars)(const char* id, const char* stdCode, const char* period, VvTSBarStruct* bars, VvTUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpTicks)(const char* id, const char* stdCode, VvTUInt32 uDate, VvTSTickStruct* ticks, VvTUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpOrdQue)(const char* id, const char* stdCode, VvTUInt32 uDate, VvTSOrdQueStruct* items, VvTUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpOrdDtl)(const char* id, const char* stdCode, VvTUInt32 uDate, VvTSOrdDtlStruct* items, VvTUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpTrans)(const char* id, const char* stdCode, VvTUInt32 uDate, VvTSTransStruct* items, VvTUInt32 count);
