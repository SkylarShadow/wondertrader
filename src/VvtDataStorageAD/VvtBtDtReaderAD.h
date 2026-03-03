#pragma once
#include <string>

#include "../Includes/FasterDefs.h"
#include "../Includes/IBtDtReader.h"

#include "../VvTSUtils/VvtLMDB.hpp"

NS_VVTP_BEGIN

class VvtBtDtReaderAD : public IBtDtReader
{
public:
	VvtBtDtReaderAD();
	virtual ~VvtBtDtReaderAD();	

//////////////////////////////////////////////////////////////////////////
//IBtDtReader
public:
	virtual void init(VvTSVariant* cfg, IBtDtReaderSink* sink);

	virtual bool read_raw_bars(const char* exchg, const char* code, VvTSKlinePeriod period, std::string& buffer) override;
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