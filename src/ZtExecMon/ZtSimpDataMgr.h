#pragma once
#include <vector>
#include "../Includes/IDataReader.h"
#include "../Includes/IDataManager.h"

#include "../Includes/ZTSCollection.hpp"

class ZtExecRunner;

NS_ZTP_BEGIN
class ZTSVariant;
class ZTSHisTickData;
class ZTSKlineData;
class ZTSTickData;
class ZTSKlineSlice;
class ZTSTickSlice;
class IBaseDataMgr;
class IBaseDataMgr;
class ZTSSessionInfo;

class ZtSimpDataMgr : public IDataReaderSink, public IDataManager
{
public:
	ZtSimpDataMgr();
	~ZtSimpDataMgr();

private:
	bool	initStore(ZTSVariant* cfg);

public:
	bool	init(ZTSVariant* cfg, ZtExecRunner* runner);

	void	handle_push_quote(const char* stdCode, ZTSTickData* newTick);

	//////////////////////////////////////////////////////////////////////////
	//IDataManager
	ZTSTickSlice* get_tick_slice(const char* code, uint32_t count, uint64_t etime = 0) override;
	ZTSKlineSlice* get_kline_slice(const char* code, ZTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) override;
	ZTSTickData* grab_last_tick(const char* code) override;

	//////////////////////////////////////////////////////////////////////////
	//IDataStoreListener
	virtual void	on_bar(const char* code, ZTSKlinePeriod period, ZTSBarStruct* newBar) override;
	virtual void	on_all_bar_updated(uint32_t updateTime) override;

	virtual IBaseDataMgr* get_basedata_mgr() override;
	virtual IHotMgr*	get_hot_mgr() override;
	virtual uint32_t	get_date() override;
	virtual uint32_t	get_min_time()override;
	virtual uint32_t	get_secs() override;

	virtual void		reader_log(ZTSLogLevel ll, const char* message) override;

	inline IDataReader* reader() { return _reader; }

	inline uint32_t	get_raw_time() const { return _cur_raw_time; }
	inline uint32_t	get_trading_day() const { return _cur_tdate; }

private:
	IDataReader*	_reader;
	ZtExecRunner*	_runner;
	ZTSSessionInfo*	_s_info;

	typedef ZTSHashMap<std::string> DataCacheMap;
	DataCacheMap* _bars_cache;	//K线缓存
	DataCacheMap* _rt_tick_map;	//实时tick缓存

	uint32_t		_cur_date;		//当前日期,格式如yyyyMMdd
	uint32_t		_cur_act_time;	//当前完整时间,格式如hhmmssmmm
	uint32_t		_cur_raw_time;	//当前真实分钟,格式如hhmm
	uint32_t		_cur_min_time;	//当前1分钟线时间,格式如hhmm
	uint32_t		_cur_secs;		//当前秒数,格式如ssmmm
	uint32_t		_cur_tdate;		//当前交易日,格式如yyyyMMdd

};

NS_ZTP_END
