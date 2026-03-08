/*!
 * \file ZtDataManager.h
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
#include "../Includes/ZTSCollection.hpp"

NS_ZTP_BEGIN
class ZTSVariant;
class ZTSTickData;
class ZTSKlineSlice;
class ZTSTickSlice;
class IBaseDataMgr;
class IBaseDataMgr;
class ZtUftEngine;

class ZtUftDtMgr : public IDataManager
{
public:
	ZtUftDtMgr();
	~ZtUftDtMgr();

public:
	bool	init(ZTSVariant* cfg, ZtUftEngine* engine);

	void	handle_push_quote(const char* stdCode, ZTSTickData* newTick);

	//////////////////////////////////////////////////////////////////////////
	//IDataManager 接口
	virtual ZTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSKlineSlice* get_kline_slice(const char* stdCode, ZTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSTickData* grab_last_tick(const char* stdCode) override;

private:
	ZtUftEngine*		_engine;

	vvt_hashset<std::string> _subed_basic_bars;
	typedef ZTSHashMap<std::string> DataCacheMap;
	DataCacheMap*	_bars_cache;	//K线缓存
	DataCacheMap*	_ticks_cache;	//历史Tick缓存
	DataCacheMap*	_rt_tick_map;	//实时tick缓存

	typedef struct _NotifyItem
	{
		std::string _code;
		std::string _period;
		uint32_t	_times;
		ZTSBarStruct* _newBar;
	} NotifyItem;

	std::vector<NotifyItem> _bar_notifies;
};

NS_ZTP_END