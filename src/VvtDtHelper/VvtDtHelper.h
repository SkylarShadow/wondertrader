/*!
 * \file VvtDtPorter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include "../Includes/VvTSTypes.h"

NS_VVTP_BEGIN
struct VvTSBarStruct;
struct VvTSTickStruct;
struct VvTSOrdDtlStruct;
struct VvTSOrdQueStruct;
struct VvTSTransStruct;
NS_VVTP_END

USING_NS_VVTP;

typedef void(PORTER_FLAG *FuncLogCallback)(VvTString message);
typedef void(PORTER_FLAG *FuncGetBarsCallback)(VvTSBarStruct* bar, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(VvTSTickStruct* tick, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetOrdDtlCallback)(VvTSOrdDtlStruct* item, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetOrdQueCallback)(VvTSOrdQueStruct* item, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTransCallback)(VvTSTransStruct* item, VvTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncCountDataCallback)(VvTUInt32 dataCnt);

//改成直接从python传内存块的方式
//typedef bool(PORTER_FLAG *FuncGetBarItem)(VvTSBarStruct* curBar,int idx);
//typedef bool(PORTER_FLAG *FuncGetTickItem)(VvTSTickStruct* curTick, int idx);

#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG	void		dump_bars(VvTString binFolder, VvTString csvFolder, VvTString strFilter = "", FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	void		dump_ticks(VvTString binFolder, VvTString csvFolder, VvTString strFilter = "", FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	void		trans_csv_bars(VvTString csvFolder, VvTString binFolder, VvTString period, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	VvTUInt32	read_dsb_ticks(VvTString tickFile, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	VvTUInt32	read_dsb_order_details(VvTString dataFile, FuncGetOrdDtlCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	VvTUInt32	read_dsb_order_queues(VvTString dataFile, FuncGetOrdQueCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	VvTUInt32	read_dsb_transactions(VvTString dataFile, FuncGetTransCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	VvTUInt32	read_dsb_bars(VvTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	VvTUInt32	read_dmb_ticks(VvTString tickFile, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	VvTUInt32	read_dmb_bars(VvTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	//EXPORT_FLAG bool		trans_bars(VvTString barFile, FuncGetBarItem getter, int count, VvTString period, FuncLogCallback cbLogger = NULL);
	//EXPORT_FLAG bool		trans_ticks(VvTString tickFile, FuncGetTickItem getter, int count, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG bool		store_bars(VvTString barFile, VvTSBarStruct* firstBar, int count, VvTString period, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_ticks(VvTString tickFile, VvTSTickStruct* firstTick, int count, FuncLogCallback cbLogger = NULL);

	//股票level2数据存储
	EXPORT_FLAG bool		store_order_details(VvTString tickFile, VvTSOrdDtlStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_order_queues(VvTString tickFile, VvTSOrdQueStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_transactions(VvTString tickFile, VvTSTransStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG VvTUInt32	resample_bars(VvTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, 
		VvTUInt64 fromTime, VvTUInt64 endTime, VvTString period, VvTUInt32 times, VvTString sessInfo, FuncLogCallback cbLogger = NULL, bool bAlignSec = false);
#ifdef __cplusplus
}
#endif