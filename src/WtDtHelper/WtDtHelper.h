/*!
 * \file WtDtPorter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include "../Includes/WTSTypes.h"

NS_VVTP_BEGIN
struct WTSBarStruct;
struct WTSTickStruct;
struct WTSOrdDtlStruct;
struct WTSOrdQueStruct;
struct WTSTransStruct;
NS_VVTP_END

USING_NS_VVTP;

typedef void(PORTER_FLAG *FuncLogCallback)(VvtString message);
typedef void(PORTER_FLAG *FuncGetBarsCallback)(WTSBarStruct* bar, VvtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(WTSTickStruct* tick, VvtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetOrdDtlCallback)(WTSOrdDtlStruct* item, VvtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetOrdQueCallback)(WTSOrdQueStruct* item, VvtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTransCallback)(WTSTransStruct* item, VvtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncCountDataCallback)(VvtUInt32 dataCnt);

//改成直接从python传内存块的方式
//typedef bool(PORTER_FLAG *FuncGetBarItem)(WTSBarStruct* curBar,int idx);
//typedef bool(PORTER_FLAG *FuncGetTickItem)(WTSTickStruct* curTick, int idx);

#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG	void		dump_bars(VvtString binFolder, VvtString csvFolder, VvtString strFilter = "", FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	void		dump_ticks(VvtString binFolder, VvtString csvFolder, VvtString strFilter = "", FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	void		trans_csv_bars(VvtString csvFolder, VvtString binFolder, VvtString period, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	VvtUInt32	read_dsb_ticks(VvtString tickFile, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	VvtUInt32	read_dsb_order_details(VvtString dataFile, FuncGetOrdDtlCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	VvtUInt32	read_dsb_order_queues(VvtString dataFile, FuncGetOrdQueCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	VvtUInt32	read_dsb_transactions(VvtString dataFile, FuncGetTransCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	VvtUInt32	read_dsb_bars(VvtString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	VvtUInt32	read_dmb_ticks(VvtString tickFile, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	VvtUInt32	read_dmb_bars(VvtString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	//EXPORT_FLAG bool		trans_bars(VvtString barFile, FuncGetBarItem getter, int count, VvtString period, FuncLogCallback cbLogger = NULL);
	//EXPORT_FLAG bool		trans_ticks(VvtString tickFile, FuncGetTickItem getter, int count, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG bool		store_bars(VvtString barFile, WTSBarStruct* firstBar, int count, VvtString period, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_ticks(VvtString tickFile, WTSTickStruct* firstTick, int count, FuncLogCallback cbLogger = NULL);

	//股票level2数据存储
	EXPORT_FLAG bool		store_order_details(VvtString tickFile, WTSOrdDtlStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_order_queues(VvtString tickFile, WTSOrdQueStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_transactions(VvtString tickFile, WTSTransStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG VvtUInt32	resample_bars(VvtString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, 
		VvtUInt64 fromTime, VvtUInt64 endTime, VvtString period, VvtUInt32 times, VvtString sessInfo, FuncLogCallback cbLogger = NULL, bool bAlignSec = false);
#ifdef __cplusplus
}
#endif