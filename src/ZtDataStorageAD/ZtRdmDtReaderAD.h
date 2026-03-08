#pragma once
#include <string>
#include <stdint.h>
#include <boost/circular_buffer.hpp>

#include "DataDefineAD.h"

#include "../ZTSUtils/ZtLMDB.hpp"
#include "../Includes/FasterDefs.h"
#include "../Includes/IRdmDtReader.h"

#include "../Share/StdUtils.hpp"
#include "../Share/BoostMappingFile.hpp"

NS_ZTP_BEGIN

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

class ZtRdmDtReaderAD : public IRdmDtReader
{
public:
	ZtRdmDtReaderAD();
	virtual ~ZtRdmDtReaderAD();


//////////////////////////////////////////////////////////////////////////
//IRdmDtReader
public:
	virtual void init(ZTSVariant* cfg, IRdmDtReaderSink* sink);

	virtual ZTSOrdDtlSlice*	readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }
	virtual ZTSOrdQueSlice*	readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }
	virtual ZTSTransSlice*	readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }

	virtual ZTSTickSlice*	readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override;
	virtual ZTSKlineSlice*	readKlineSliceByRange(const char* stdCode, ZTSKlinePeriod period, uint64_t stime, uint64_t etime = 0) override;

	virtual ZTSTickSlice*	readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSKlineSlice*	readKlineSliceByCount(const char* stdCode, ZTSKlinePeriod period, uint32_t count, uint64_t etime = 0) override;

	virtual ZTSTickSlice*	readTickSliceByDate(const char* stdCode, uint32_t uDate /* = 0 */) override;

private:
	std::string		_base_dir;
	IBaseDataMgr*	_base_data_mgr;
	IHotMgr*		_hot_mgr;

	typedef struct _BarsList
	{
		std::string		_exchg;
		std::string		_code;
		ZTSKlinePeriod	_period;
		uint64_t		_last_bar_time;

		std::vector<ZTSBarStruct>	_bars;

		_BarsList() :_last_bar_time(0){}
	} BarsList;

	typedef struct _TicksList
	{
		std::string		_exchg;
		std::string		_code;
		uint64_t		_first_tick_time;
		uint64_t		_last_tick_time;

		std::vector<ZTSTickStruct>	_ticks;

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
	typedef std::shared_ptr<ZtLMDB> ZtLMDBPtr;
	typedef vvt_hashmap<std::string, ZtLMDBPtr> ZtLMDBMap;

	ZtLMDBMap	_exchg_m1_dbs;
	ZtLMDBMap	_exchg_m5_dbs;
	ZtLMDBMap	_exchg_d1_dbs;

	//用exchg.code作为key，如BINANCE.BTCUSDT
	ZtLMDBMap	_tick_dbs;

	ZtLMDBPtr	get_k_db(const char* exchg, ZTSKlinePeriod period);

	ZtLMDBPtr	get_t_db(const char* exchg, const char* code);
};

NS_ZTP_END
