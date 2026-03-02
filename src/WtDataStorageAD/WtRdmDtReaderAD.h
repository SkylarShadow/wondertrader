#pragma once
#include <string>
#include <stdint.h>
#include <boost/circular_buffer.hpp>

#include "DataDefineAD.h"

#include "../VvTSUtils/VvtLMDB.hpp"
#include "../Includes/FasterDefs.h"
#include "../Includes/IRdmDtReader.h"

#include "../Share/StdUtils.hpp"
#include "../Share/BoostMappingFile.hpp"

NS_VVTP_BEGIN

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

class WtRdmDtReaderAD : public IRdmDtReader
{
public:
	WtRdmDtReaderAD();
	virtual ~WtRdmDtReaderAD();


//////////////////////////////////////////////////////////////////////////
//IRdmDtReader
public:
	virtual void init(VvTSVariant* cfg, IRdmDtReaderSink* sink);

	virtual VvTSOrdDtlSlice*	readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }
	virtual VvTSOrdQueSlice*	readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }
	virtual VvTSTransSlice*	readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }

	virtual VvTSTickSlice*	readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override;
	virtual VvTSKlineSlice*	readKlineSliceByRange(const char* stdCode, VvTSKlinePeriod period, uint64_t stime, uint64_t etime = 0) override;

	virtual VvTSTickSlice*	readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual VvTSKlineSlice*	readKlineSliceByCount(const char* stdCode, VvTSKlinePeriod period, uint32_t count, uint64_t etime = 0) override;

	virtual VvTSTickSlice*	readTickSliceByDate(const char* stdCode, uint32_t uDate /* = 0 */) override;

private:
	std::string		_base_dir;
	IBaseDataMgr*	_base_data_mgr;
	IHotMgr*		_hot_mgr;

	typedef struct _BarsList
	{
		std::string		_exchg;
		std::string		_code;
		VvTSKlinePeriod	_period;
		uint64_t		_last_bar_time;

		std::vector<VvTSBarStruct>	_bars;

		_BarsList() :_last_bar_time(0){}
	} BarsList;

	typedef struct _TicksList
	{
		std::string		_exchg;
		std::string		_code;
		uint64_t		_first_tick_time;
		uint64_t		_last_tick_time;

		std::vector<VvTSTickStruct>	_ticks;

		_TicksList() :_last_tick_time(0), _first_tick_time(UINT64_MAX){}
	} TicksList;

	typedef vvt_hashmap<std::string, BarsList> BarsCache;
	BarsCache	_bars_cache;

	typedef vvt_hashmap<std::string, TicksList> TicksCache;
	TicksCache	_ticks_cache;

private:
	//////////////////////////////////////////////////////////////////////////
	/*
	 *	这里放LMDB的数据库定义
	 *	K线数据，按照每个市场m1/m5/d1三个周期一共三个数据库，路径如./m1/CFFEX
	 *	Tick数据，每个合约一个数据库，路径如./ticks/CFFEX/IF2101
	 */
	typedef std::shared_ptr<VvtLMDB> VvtLMDBPtr;
	typedef vvt_hashmap<std::string, VvtLMDBPtr> VvtLMDBMap;

	VvtLMDBMap	_exchg_m1_dbs;
	VvtLMDBMap	_exchg_m5_dbs;
	VvtLMDBMap	_exchg_d1_dbs;

	//用exchg.code作为key，如BINANCE.BTCUSDT
	VvtLMDBMap	_tick_dbs;

	VvtLMDBPtr	get_k_db(const char* exchg, VvTSKlinePeriod period);

	VvtLMDBPtr	get_t_db(const char* exchg, const char* code);
};

NS_VVTP_END
