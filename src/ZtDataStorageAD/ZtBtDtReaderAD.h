#pragma once
#include <string>

#include "../Includes/FasterDefs.h"
#include "../Includes/IBtDtReader.h"

#include "../ZTSUtils/ZtLMDB.hpp"

NS_ZTP_BEGIN

class ZtBtDtReaderAD : public IBtDtReader
{
public:
	ZtBtDtReaderAD();
	virtual ~ZtBtDtReaderAD();	

//////////////////////////////////////////////////////////////////////////
//IBtDtReader
public:
	virtual void init(ZTSVariant* cfg, IBtDtReaderSink* sink);

	virtual bool read_raw_bars(const char* exchg, const char* code, ZTSKlinePeriod period, std::string& buffer) override;
	virtual bool read_raw_ticks(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) override;

private:
	std::string		_base_dir;

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