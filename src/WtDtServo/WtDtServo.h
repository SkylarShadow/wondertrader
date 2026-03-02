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
#include "PorterDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG void		initialize(VvtString cfgFile, bool isFile, VvtString logCfg, FuncOnTickCallback cbTick, FuncOnBarCallback cbBar);

	EXPORT_FLAG	VvtString	get_version();

	EXPORT_FLAG	VvtUInt32	get_bars_by_range(const char* stdCode, const char* period, VvtUInt64 beginTime, VvtUInt64 endTime, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	VvtUInt32	get_ticks_by_range(const char* stdCode, VvtUInt64 beginTime, VvtUInt64 endTime, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	VvtUInt32	get_bars_by_count(const char* stdCode, const char* period, VvtUInt32 count, VvtUInt64 endTime, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	VvtUInt32	get_ticks_by_count(const char* stdCode, VvtUInt32 count, VvtUInt64 endTime, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	VvtUInt32	get_ticks_by_date(const char* stdCode, VvtUInt32 uDate, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	VvtUInt32	get_sbars_by_date(const char* stdCode, VvtUInt32 secs, VvtUInt32 uDate, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	VvtUInt32	get_bars_by_date(const char* stdCode, const char* period, VvtUInt32 uDate, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG void		subscribe_tick(const char* stdCode, bool bReplace);

	EXPORT_FLAG void		subscribe_bar(const char* stdCode, const char* period);

	EXPORT_FLAG void		clear_cache();

#ifdef __cplusplus
}
#endif