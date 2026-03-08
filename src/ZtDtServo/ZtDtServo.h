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
#include "PorterDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG void		initialize(ZTString cfgFile, bool isFile, ZTString logCfg, FuncOnTickCallback cbTick, FuncOnBarCallback cbBar);

	EXPORT_FLAG	ZTString	get_version();

	EXPORT_FLAG	ZTUInt32	get_bars_by_range(const char* stdCode, const char* period, ZTUInt64 beginTime, ZTUInt64 endTime, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	ZTUInt32	get_ticks_by_range(const char* stdCode, ZTUInt64 beginTime, ZTUInt64 endTime, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	ZTUInt32	get_bars_by_count(const char* stdCode, const char* period, ZTUInt32 count, ZTUInt64 endTime, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	ZTUInt32	get_ticks_by_count(const char* stdCode, ZTUInt32 count, ZTUInt64 endTime, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	ZTUInt32	get_ticks_by_date(const char* stdCode, ZTUInt32 uDate, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	ZTUInt32	get_sbars_by_date(const char* stdCode, ZTUInt32 secs, ZTUInt32 uDate, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	ZTUInt32	get_bars_by_date(const char* stdCode, const char* period, ZTUInt32 uDate, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG void		subscribe_tick(const char* stdCode, bool bReplace);

	EXPORT_FLAG void		subscribe_bar(const char* stdCode, const char* period);

	EXPORT_FLAG void		clear_cache();

#ifdef __cplusplus
}
#endif