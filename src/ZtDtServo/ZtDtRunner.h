/*!
 * \file ZtDtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../ZTSTools/ZTSHotMgr.h"
#include "../ZTSTools/ZTSBaseDataMgr.h"
#include "../Share/StdUtils.hpp"

#include "PorterDefs.h"
#include "ParserAdapter.h"
#include "ZtDataManager.h"

NS_ZTP_BEGIN
class ZTSVariant;
class ZtDataStorage;
class ZTSKlineSlice;
class ZTSTickSlice;
NS_ZTP_END

class ZtDtRunner
{
public:
	ZtDtRunner();
	~ZtDtRunner();

public:
	void	initialize(const char* cfgFile, bool isFile = true, const char* modDir = "", const char* logCfg = "logcfg.yaml", 
				FuncOnTickCallback cbTick = NULL, FuncOnBarCallback cbBar = NULL);
	void	start();

	inline ZTSBaseDataMgr& getBaseDataMgr() { return _bd_mgr; }
	inline ZTSHotMgr& getHotMgr() { return _hot_mgr; }

public:
	void	proc_tick(ZTSTickData* curTick);
	void	trigger_tick(const char* stdCode, ZTSTickData* curTick);
	void	trigger_bar(const char* stdCode, const char* period, ZTSBarStruct* lastBar);
	void	sub_tick(const char* stdCode, bool bReplace, bool bInner = false);
	void	sub_bar(const char* stdCode, const char* period);

	void	clear_cache();

public:
	ZTSKlineSlice*	get_bars_by_range(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime = 0);

	ZTSKlineSlice*	get_bars_by_date(const char* stdCode, const char* period, uint32_t uDate = 0);

	ZTSTickSlice*	get_ticks_by_range(const char* stdCode, uint64_t beginTime, uint64_t endTime = 0);

	ZTSKlineSlice*	get_bars_by_count(const char* stdCode, const char* period, uint32_t count, uint64_t endTime = 0);

	ZTSTickSlice*	get_ticks_by_count(const char* stdCode, uint32_t count, uint64_t endTime = 0);

	ZTSTickSlice*	get_ticks_by_date(const char* stdCode, uint32_t uDate = 0);

	ZTSKlineSlice*	get_sbars_by_date(const char* stdCode, uint32_t secs, uint32_t uDate = 0);

private:
	void	initDataMgr(ZTSVariant* config);
	void	initParsers(ZTSVariant* cfg);

private:
	FuncOnTickCallback	_cb_tick;
	FuncOnBarCallback	_cb_bar;
	ZTSBaseDataMgr	_bd_mgr;
	ZTSHotMgr		_hot_mgr;

	ZtDataStorage*	_data_store;
	ZtDataManager	_data_mgr;
	ParserAdapterMgr	_parsers;

	bool			_is_inited;

	typedef std::set<uint32_t> SubFlags;
	typedef vvt_hashmap<std::string, SubFlags>	StraSubMap;
	StraSubMap		_tick_sub_map;	//tick数据订阅表
	StdUniqueMutex	_mtx_subs;

	StraSubMap		_tick_innersub_map;	//tick数据订阅表
	StdUniqueMutex	_mtx_innersubs;
};

