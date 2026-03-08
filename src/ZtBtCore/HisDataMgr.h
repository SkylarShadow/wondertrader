#pragma once
#include <functional>
#include "../Includes/IBtDtReader.h"

typedef std::function<void(std::string&)> FuncLoadDataCallback;

NS_ZTP_BEGIN
class ZTSVariant;
NS_ZTP_END

USING_NS_ZTP;

class HisDataMgr : public IBtDtReaderSink
{
public:
	HisDataMgr() :_reader(NULL) {}
	~HisDataMgr(){}

public:
	virtual void reader_log(ZTSLogLevel ll, const char* message) override;

public:
	bool	init(ZTSVariant* cfg);

	bool	load_raw_bars(const char* exchg, const char* code, ZTSKlinePeriod period, FuncLoadDataCallback cb);

	bool	load_raw_ticks(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb);

	bool	load_raw_ordque(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb);

	bool	load_raw_orddtl(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb);

	bool	load_raw_trans(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb);

private:
	IBtDtReader*	_reader;
};

