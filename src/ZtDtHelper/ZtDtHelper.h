/*!
 * \file ZtDtPorter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include "../Includes/ZTSTypes.h"

NS_ZTP_BEGIN
struct ZTSBarStruct;
struct ZTSTickStruct;
struct ZTSOrdDtlStruct;
struct ZTSOrdQueStruct;
struct ZTSTransStruct;
NS_ZTP_END

USING_NS_ZTP;

typedef void(PORTER_FLAG *FuncLogCallback)(ZTString message);
typedef void(PORTER_FLAG *FuncGetBarsCallback)(ZTSBarStruct* bar, ZTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(ZTSTickStruct* tick, ZTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetOrdDtlCallback)(ZTSOrdDtlStruct* item, ZTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetOrdQueCallback)(ZTSOrdQueStruct* item, ZTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTransCallback)(ZTSTransStruct* item, ZTUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncCountDataCallback)(ZTUInt32 dataCnt);

//改成直接从python传内存块的方式
//typedef bool(PORTER_FLAG *FuncGetBarItem)(ZTSBarStruct* curBar,int idx);
//typedef bool(PORTER_FLAG *FuncGetTickItem)(ZTSTickStruct* curTick, int idx);

#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG	void		dump_bars(ZTString binFolder, ZTString csvFolder, ZTString strFilter = "", FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	void		dump_ticks(ZTString binFolder, ZTString csvFolder, ZTString strFilter = "", FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	void		trans_csv_bars(ZTString csvFolder, ZTString binFolder, ZTString period, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	ZTUInt32	read_dsb_ticks(ZTString tickFile, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	ZTUInt32	read_dsb_order_details(ZTString dataFile, FuncGetOrdDtlCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	ZTUInt32	read_dsb_order_queues(ZTString dataFile, FuncGetOrdQueCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	ZTUInt32	read_dsb_transactions(ZTString dataFile, FuncGetTransCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	ZTUInt32	read_dsb_bars(ZTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	ZTUInt32	read_dmb_ticks(ZTString tickFile, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	ZTUInt32	read_dmb_bars(ZTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger = NULL);

	//EXPORT_FLAG bool		trans_bars(ZTString barFile, FuncGetBarItem getter, int count, ZTString period, FuncLogCallback cbLogger = NULL);
	//EXPORT_FLAG bool		trans_ticks(ZTString tickFile, FuncGetTickItem getter, int count, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG bool		store_bars(ZTString barFile, ZTSBarStruct* firstBar, int count, ZTString period, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_ticks(ZTString tickFile, ZTSTickStruct* firstTick, int count, FuncLogCallback cbLogger = NULL);

	//股票level2数据存储
	EXPORT_FLAG bool		store_order_details(ZTString tickFile, ZTSOrdDtlStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_order_queues(ZTString tickFile, ZTSOrdQueStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG bool		store_transactions(ZTString tickFile, ZTSTransStruct* firstItem, int count, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG ZTUInt32	resample_bars(ZTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, 
		ZTUInt64 fromTime, ZTUInt64 endTime, ZTString period, ZTUInt32 times, ZTString sessInfo, FuncLogCallback cbLogger = NULL, bool bAlignSec = false);
#ifdef __cplusplus
}
#endif