#pragma once
#include "../Includes/ZTSTypes.h"

NS_ZTP_BEGIN
class ZTSTickSlice;
class ZTSKlineSlice;
class ZTSTickData;
class ZTSOrdQueSlice;
class ZTSOrdDtlSlice;
class ZTSTransSlice;


class IDataManager
{
public:
	virtual ZTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual ZTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual ZTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual ZTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual ZTSKlineSlice* get_kline_slice(const char* stdCode, ZTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) { return NULL; }

	virtual ZTSTickData* grab_last_tick(const char* stdCode) { return NULL; }

	virtual double get_adjusting_factor(const char* stdCode, uint32_t uDate) { return 1.0; }

	virtual uint32_t get_adjusting_flag() { return 0; }
};

NS_ZTP_END
