#pragma once
#include <functional>
#include "../Includes/IBtDtReader.h"

typedef std::function<void(std::string&)> FuncLoadDataCallback;

NS_VVTP_BEGIN
class VvTSVariant;
NS_VVTP_END

USING_NS_VVTP;

class HisDataMgr : public IBtDtReaderSink
{
public:
	HisDataMgr() :_reader(NULL) {}
	~HisDataMgr(){}

public:
	virtual void reader_log(VvTSLogLevel ll, const char* message) override;

public:
	bool	init(VvTSVariant* cfg);

	bool	load_raw_bars(const char* exchg, const char* code, VvTSKlinePeriod period, FuncLoadDataCallback cb);

	bool	load_raw_ticks(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb);

	bool	load_raw_ordque(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb);

	bool	load_raw_orddtl(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb);

	bool	load_raw_trans(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb);

private:
	IBtDtReader*	_reader;
};

