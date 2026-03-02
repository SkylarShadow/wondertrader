#pragma once
#include "../Includes/VvTSTypes.h"

NS_VVTP_BEGIN
class VvTSTickSlice;
class VvTSKlineSlice;
class VvTSTickData;
class VvTSOrdQueSlice;
class VvTSOrdDtlSlice;
class VvTSTransSlice;


class IDataManager
{
public:
	virtual VvTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual VvTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual VvTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual VvTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) { return NULL; }
	virtual VvTSKlineSlice* get_kline_slice(const char* stdCode, VvTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) { return NULL; }

	virtual VvTSTickData* grab_last_tick(const char* stdCode) { return NULL; }

	virtual double get_adjusting_factor(const char* stdCode, uint32_t uDate) { return 1.0; }

	virtual uint32_t get_adjusting_flag() { return 0; }
};

NS_VVTP_END
