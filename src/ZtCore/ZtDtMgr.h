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
class ZtEngine;

class ZtDtMgr : public IDataReaderSink, public IDataManager
{
public:
	ZtDtMgr();
	~ZtDtMgr();

private:
	bool	initStore(ZTSVariant* cfg);

public:
	bool	init(ZTSVariant* cfg, ZtEngine* engine, bool bForceCache = false);

	void	regsiter_loader(IHisDataLoader* loader) { _loader = loader; }

	void	handle_push_quote(const char* stdCode, ZTSTickData* newTick);

	//////////////////////////////////////////////////////////////////////////
	//IDataManager 接口
	virtual ZTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSKlineSlice* get_kline_slice(const char* stdCode, ZTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) override;
	virtual ZTSTickData* grab_last_tick(const char* stdCode) override;
	virtual double get_adjusting_factor(const char* stdCode, uint32_t uDate) override;

	virtual uint32_t get_adjusting_flag() override;

	//////////////////////////////////////////////////////////////////////////
	//IDataReaderSink
	virtual void	on_bar(const char* code, ZTSKlinePeriod period, ZTSBarStruct* newBar) override;
	virtual void	on_all_bar_updated(uint32_t updateTime) override;

	virtual IBaseDataMgr*	get_basedata_mgr() override;
	virtual IHotMgr*		get_hot_mgr() override;
	virtual uint32_t	get_date() override;
	virtual uint32_t	get_min_time()override;
	virtual uint32_t	get_secs() override;

	virtual void		reader_log(ZTSLogLevel ll, const char* message) override;

	inline IDataReader*	reader() { return _reader; }
	inline IHisDataLoader*	loader() { return _loader; }

private:
	IDataReader*	_reader;
	IHisDataLoader*	_loader;
	ZtEngine*		_engine;

	bool			_align_by_section;	//强制小节对齐
	bool			_force_cache;		//强制缓存K线

	vvt_hashset<std::string> _subed_basic_bars;
	typedef ZTSHashMap<std::string> DataCacheMap;
	DataCacheMap*	_bars_cache;	//K线缓存
	DataCacheMap*	_rt_tick_map;	//实时tick缓存
	//By Wesley @ 2022.02.11
	//这个只有后复权tick数据
	//因为前复权和不复权，都不需要缓存
	DataCacheMap*	_ticks_adjusted;	//复权tick缓存

	typedef struct _NotifyItem
	{
		char		_code[MAX_INSTRUMENT_LENGTH];
		char		_period[2] = { 0 };
		uint32_t	_times;
		ZTSBarStruct* _newBar;

		_NotifyItem(const char* code, char period, uint32_t times, ZTSBarStruct* newBar)
			: _times(times), _newBar(newBar)
		{
			vvt_strcpy(_code, code);
			_period[0] = period;
		}
	} NotifyItem;

	std::vector<NotifyItem> _bar_notifies;
};

NS_ZTP_END