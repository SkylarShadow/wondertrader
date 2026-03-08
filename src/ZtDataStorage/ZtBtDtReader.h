#pragma once
#include <string>
#include <stdint.h>

#include "DataDefine.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/IBtDtReader.h"

#include "../Share/BoostMappingFile.hpp"
#include "../Share/StdUtils.hpp"

NS_ZTP_BEGIN
class ZTSVariant;
class ZTSTickSlice;
class ZTSKlineSlice;
class ZTSOrdDtlSlice;
class ZTSOrdQueSlice;
class ZTSTransSlice;
class ZTSArray;

class IBaseDataMgr;
class IHotMgr;
typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

class ZtBtDtReader : public IBtDtReader
{
public:
	ZtBtDtReader();
	virtual ~ZtBtDtReader();	

//////////////////////////////////////////////////////////////////////////
//IBtDtReader
public:
	virtual void init(ZTSVariant* cfg, IBtDtReaderSink* sink);

	virtual bool read_raw_bars(const char* exchg, const char* code, ZTSKlinePeriod period, std::string& buffer) override;
	virtual bool read_raw_ticks(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) override;

	virtual bool read_raw_order_details(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) override;
	virtual bool read_raw_order_queues(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) override;
	virtual bool read_raw_transactions(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) override;

private:
	std::string		_base_dir;
};

NS_ZTP_END