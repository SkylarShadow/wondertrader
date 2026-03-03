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
struct VvTSOrdDtlStruct;
struct VvTSOrdQueStruct;
struct VvTSTransStruct;
NS_VVTP_END

USING_NS_VVTP;

typedef unsigned long		CtxHandler;

static const VvTUInt32	EVENT_ENGINE_INIT	= 1;	//框架初始化
static const VvTUInt32	EVENT_SESSION_BEGIN = 2;	//交易日开始
static const VvTUInt32	EVENT_SESSION_END	= 3;	//交易日结束
static const VvTUInt32	EVENT_ENGINE_SCHDL	= 4;	//框架调度

static const VvTUInt32	CHNL_EVENT_READY	= 1000;	//通道就绪事件
static const VvTUInt32	CHNL_EVENT_LOST		= 1001;	//通道断开事件

//日志级别
static const VvTUInt32	LOG_LEVEL_DEBUG		= 0;
static const VvTUInt32	LOG_LEVEL_INFO		= 1;
static const VvTUInt32	LOG_LEVEL_WARN		= 2;
static const VvTUInt32	LOG_LEVEL_ERROR		= 3;


/*
*	回调函数定义
*/
typedef void(PORTER_FLAG *FuncGetBarsCallback)(CtxHandler cHandle, const char* stdCode, const char* period, VvTSBarStruct* bar, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(CtxHandler cHandle, const char* stdCode, VvTSTickStruct* tick, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraInitCallback)(CtxHandler cHandle);
typedef void(PORTER_FLAG *FuncSessionEvtCallback)(CtxHandler cHandle, VvTUInt32 curTDate, bool isBegin);
typedef void(PORTER_FLAG *FuncStraTickCallback)(CtxHandler cHandle, const char* stdCode, VvTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncStraCalcCallback)(CtxHandler cHandle, VvTUInt32 curDate, VvTUInt32 curTime);
typedef void(PORTER_FLAG *FuncStraBarCallback)(CtxHandler cHandle, const char* stdCode, const char* period, VvTSBarStruct* newBar);
typedef void(PORTER_FLAG *FuncGetPositionCallback)(CtxHandler cHandle, const char* stdCode, double position, bool isLast);
typedef void(PORTER_FLAG *FuncStraCondTriggerCallback)(CtxHandler cHandle, const char* stdCode, double target, double price, const char* usertag);

typedef void(PORTER_FLAG *FuncStraOrdQueCallback)(CtxHandler cHandle, const char* stdCode, VvTSOrdQueStruct* ordQue);
typedef void(PORTER_FLAG *FuncGetOrdQueCallback)(CtxHandler cHandle, const char* stdCode, VvTSOrdQueStruct* ordQue, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraOrdDtlCallback)(CtxHandler cHandle, const char* stdCode, VvTSOrdDtlStruct* ordDtl);
typedef void(PORTER_FLAG *FuncGetOrdDtlCallback)(CtxHandler cHandle, const char* stdCode, VvTSOrdDtlStruct* ordDtl, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraTransCallback)(CtxHandler cHandle, const char* stdCode, VvTSTransStruct* trans);
typedef void(PORTER_FLAG *FuncGetTransCallback)(CtxHandler cHandle, const char* stdCode, VvTSTransStruct* trans, VvTUInt32 count, bool isLast);

//////////////////////////////////////////////////////////////////////////
//HFT回调函数
typedef void(PORTER_FLAG *FuncHftChannelCallback)(CtxHandler cHandle, const char* trader, VvTUInt32 evtid);	//交易通道事件回调
typedef void(PORTER_FLAG *FuncHftOrdCallback)(CtxHandler cHandle, VvTUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag);
typedef void(PORTER_FLAG *FuncHftTrdCallback)(CtxHandler cHandle, VvTUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag);
typedef void(PORTER_FLAG *FuncHftEntrustCallback)(CtxHandler cHandle, VvTUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag);
typedef void(PORTER_FLAG *FuncHftPosCallback)(CtxHandler cHandle, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail);


typedef void(PORTER_FLAG *FuncEventCallback)(VvTUInt32 evtId, VvTUInt32 curDate, VvTUInt32 curTime);

//////////////////////////////////////////////////////////////////////////
//扩展Parser回调函数
static const VvTUInt32	EVENT_PARSER_INIT		= 1;	//Parser初始化
static const VvTUInt32	EVENT_PARSER_CONNECT	= 2;	//Parser连接
static const VvTUInt32	EVENT_PARSER_DISCONNECT = 3;	//Parser断开连接
static const VvTUInt32	EVENT_PARSER_RELEASE	= 4;	//Parser释放

typedef void(PORTER_FLAG *FuncParserEvtCallback)(VvTUInt32 evtId, const char* id);
typedef void(PORTER_FLAG *FuncParserSubCallback)(const char* id, const char* fullCode, bool isForSub);

//////////////////////////////////////////////////////////////////////////
//扩展Executer回调函数
typedef void(PORTER_FLAG *FuncExecInitCallback)(const char* id);
typedef void(PORTER_FLAG *FuncExecCmdCallback)(const char* id, const char* StdCode, double targetPos);

//////////////////////////////////////////////////////////////////////////
//外部数据加载模块
typedef bool(PORTER_FLAG *FuncLoadFnlBars)(const char* stdCode, const char* period);
typedef bool(PORTER_FLAG *FuncLoadRawBars)(const char* stdCode, const char* period);
typedef bool(PORTER_FLAG *FuncLoadAdjFactors)(const char* stdCode);
typedef bool(PORTER_FLAG *FuncLoadRawTicks)(const char* stdCode, uint32_t uDate);
