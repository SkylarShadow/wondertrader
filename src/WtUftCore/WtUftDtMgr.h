/*!
 * \file WtDataManager.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <vector>
#include "../Includes/IDataReader.h"
#include "../Includes/IDataManager.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/VvTSCollection.hpp"

NS_VVTP_BEGIN
class VvTSVariant;
class VvTSTickData;
class VvTSKlineSlice;
class VvTSTickSlice;
class IBaseDataMgr;
class IBaseDataMgr;
class WtUftEngine;

class WtUftDtMgr : public IDataManager
{
public:
	WtUftDtMgr();
	~WtUftDtMgr();

public:
	bool	init(VvTSVariant* cfg, WtUftEngine* engine);

	void	handle_push_quote(const char* stdCode, VvTSTickData* newTick);

	//////////////////////////////////////////////////////////////////////////
	//IDataManager 接口
	virtual VvTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual VvTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual VvTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual VvTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual VvTSKlineSlice* get_kline_slice(const char* stdCode, VvTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) override;
	virtual VvTSTickData* grab_last_tick(const char* stdCode) override;

private:
	WtUftEngine*		_engine;

	vvt_hashset<std::string> _subed_basic_bars;
	typedef VvTSHashMap<std::string> DataCacheMap;
	DataCacheMap*	_bars_cache;	//K线缓存
	DataCacheMap*	_ticks_cache;	//历史Tick缓存
	DataCacheMap*	_rt_tick_map;	//实时tick缓存

	typedef struct _NotifyItem
	{
		std::string _code;
		std::string _period;
		uint32_t	_times;
		VvTSBarStruct* _newBar;
	} NotifyItem;

	std::vector<NotifyItem> _bar_notifies;
};

NS_VVTP_END