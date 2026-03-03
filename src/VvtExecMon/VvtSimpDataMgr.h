#pragma once
#include <vector>
#include "../Includes/IDataReader.h"
#include "../Includes/IDataManager.h"

#include "../Includes/VvTSCollection.hpp"

class VvtExecRunner;

NS_VVTP_BEGIN
class VvTSVariant;
class VvTSHisTickData;
class VvTSKlineData;
class VvTSTickData;
class VvTSKlineSlice;
class VvTSTickSlice;
class IBaseDataMgr;
class IBaseDataMgr;
class VvTSSessionInfo;

class VvtSimpDataMgr : public IDataReaderSink, public IDataManager
{
public:
	VvtSimpDataMgr();
	~VvtSimpDataMgr();

private:
	bool	initStore(VvTSVariant* cfg);

public:
	bool	init(VvTSVariant* cfg, VvtExecRunner* runner);

	void	handle_push_quote(const char* stdCode, VvTSTickData* newTick);

	//////////////////////////////////////////////////////////////////////////
	//IDataManager
	VvTSTickSlice* get_tick_slice(const char* code, uint32_t count, uint64_t etime = 0) override;
	VvTSKlineSlice* get_kline_slice(const char* code, VvTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) override;
	VvTSTickData* grab_last_tick(const char* code) override;

	//////////////////////////////////////////////////////////////////////////
	//IDataStoreListener
	virtual void	on_bar(const char* code, VvTSKlinePeriod period, VvTSBarStruct* newBar) override;
	virtual void	on_all_bar_updated(uint32_t updateTime) override;

	virtual IBaseDataMgr* get_basedata_mgr() override;
	virtual IHotMgr*	get_hot_mgr() override;
	virtual uint32_t	get_date() override;
	virtual uint32_t	get_min_time()override;
	virtual uint32_t	get_secs() override;

	virtual void		reader_log(VvTSLogLevel ll, const char* message) override;

	inline IDataReader* reader() { return _reader; }

	inline uint32_t	get_raw_time() const { return _cur_raw_time; }
	inline uint32_t	get_trading_day() const { return _cur_tdate; }

private:
	IDataReader*	_reader;
	VvtExecRunner*	_runner;
	VvTSSessionInfo*	_s_info;

	typedef VvTSHashMap<std::string> DataCacheMap;
	DataCacheMap* _bars_cache;	//K线缓存
	DataCacheMap* _rt_tick_map;	//实时tick缓存

	uint32_t		_cur_date;		//当前日期,格式如yyyyMMdd
	uint32_t		_cur_act_time;	//当前完整时间,格式如hhmmssmmm
	uint32_t		_cur_raw_time;	//当前真实分钟,格式如hhmm
	uint32_t		_cur_min_time;	//当前1分钟线时间,格式如hhmm
	uint32_t		_cur_secs;		//当前秒数,格式如ssmmm
	uint32_t		_cur_tdate;		//当前交易日,格式如yyyyMMdd

};

NS_VVTP_END
