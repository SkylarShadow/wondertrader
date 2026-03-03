/*!
 * \file VvtDtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../VvTSTools/VvTSHotMgr.h"
#include "../VvTSTools/VvTSBaseDataMgr.h"
#include "../Share/StdUtils.hpp"

#include "PorterDefs.h"
#include "ParserAdapter.h"
#include "VvtDataManager.h"

NS_VVTP_BEGIN
class VvTSVariant;
class VvtDataStorage;
class VvTSKlineSlice;
class VvTSTickSlice;
NS_VVTP_END

class VvtDtRunner
{
public:
	VvtDtRunner();
	~VvtDtRunner();

public:
	void	initialize(const char* cfgFile, bool isFile = true, const char* modDir = "", const char* logCfg = "logcfg.yaml", 
				FuncOnTickCallback cbTick = NULL, FuncOnBarCallback cbBar = NULL);
	void	start();

	inline VvTSBaseDataMgr& getBaseDataMgr() { return _bd_mgr; }
	inline VvTSHotMgr& getHotMgr() { return _hot_mgr; }

public:
	void	proc_tick(VvTSTickData* curTick);
	void	trigger_tick(const char* stdCode, VvTSTickData* curTick);
	void	trigger_bar(const char* stdCode, const char* period, VvTSBarStruct* lastBar);
	void	sub_tick(const char* stdCode, bool bReplace, bool bInner = false);
	void	sub_bar(const char* stdCode, const char* period);

	void	clear_cache();

public:
	VvTSKlineSlice*	get_bars_by_range(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime = 0);

	VvTSKlineSlice*	get_bars_by_date(const char* stdCode, const char* period, uint32_t uDate = 0);

	VvTSTickSlice*	get_ticks_by_range(const char* stdCode, uint64_t beginTime, uint64_t endTime = 0);

	VvTSKlineSlice*	get_bars_by_count(const char* stdCode, const char* period, uint32_t count, uint64_t endTime = 0);

	VvTSTickSlice*	get_ticks_by_count(const char* stdCode, uint32_t count, uint64_t endTime = 0);

	VvTSTickSlice*	get_ticks_by_date(const char* stdCode, uint32_t uDate = 0);

	VvTSKlineSlice*	get_sbars_by_date(const char* stdCode, uint32_t secs, uint32_t uDate = 0);

private:
	void	initDataMgr(VvTSVariant* config);
	void	initParsers(VvTSVariant* cfg);

private:
	FuncOnTickCallback	_cb_tick;
	FuncOnBarCallback	_cb_bar;
	VvTSBaseDataMgr	_bd_mgr;
	VvTSHotMgr		_hot_mgr;

	VvtDataStorage*	_data_store;
	VvtDataManager	_data_mgr;
	ParserAdapterMgr	_parsers;

	bool			_is_inited;

	typedef std::set<uint32_t> SubFlags;
	typedef vvt_hashmap<std::string, SubFlags>	StraSubMap;
	StraSubMap		_tick_sub_map;	//tick数据订阅表
	StdUniqueMutex	_mtx_subs;

	StraSubMap		_tick_innersub_map;	//tick数据订阅表
	StdUniqueMutex	_mtx_innersubs;
};

