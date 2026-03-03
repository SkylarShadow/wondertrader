/*!
 * \file VvtDataManager.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <vector>
#include <stdint.h>

#include "../Includes/IDataManager.h"
#include "../Includes/IRdmDtReader.h"
#include "../Includes/FasterDefs.h"
#include "../Includes/VvTSCollection.hpp"
#include "../Share/StdUtils.hpp"


class VvtDtRunner;

NS_VVTP_BEGIN
class VvTSVariant;
class VvTSTickData;
class VvTSKlineSlice;
class VvTSKlineData;
class VvTSTickSlice;
class IBaseDataMgr;
class IHotMgr;
class VvTSSessionInfo;
struct VvTSBarStruct;
class VvTSCommodityInfo;

class VvtDataManager : public IRdmDtReaderSink
{
public:
	VvtDataManager();
	~VvtDataManager();

private:
	bool	initStore(VvTSVariant* cfg);

	VvTSSessionInfo* get_session_info(const char* sid, bool isCode = false);

//////////////////////////////////////////////////////////////////////////
//IRdmDtReaderSink
public:
	/*
	 *	@brief	获取基础数据管理接口指针
	 */
	virtual IBaseDataMgr*	get_basedata_mgr() override { return _bd_mgr; }

	/*
	 *	@brief	获取主力切换规则管理接口指针
	 */
	virtual IHotMgr*		get_hot_mgr() override { return _hot_mgr; }

	/*
	 *	@brief	输出数据读取模块的日志
	 */
	virtual void			reader_log(VvTSLogLevel ll, const char* message) override;

public:
	bool	init(VvTSVariant* cfg, VvtDtRunner* runner);

	VvTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	VvTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	VvTSTransSlice* get_transaction_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);

	VvTSTickSlice* get_tick_slice_by_date(const char* stdCode, uint32_t uDate = 0);
	VvTSKlineSlice* get_skline_slice_by_date(const char* stdCode, uint32_t secs, uint32_t uDate = 0);
	VvTSKlineSlice* get_kline_slice_by_date(const char* stdCode, VvTSKlinePeriod period, uint32_t times, uint32_t uDate = 0);

	VvTSTickSlice* get_tick_slices_by_range(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	VvTSKlineSlice* get_kline_slice_by_range(const char* stdCode, VvTSKlinePeriod period, uint32_t times, uint64_t stime, uint64_t etime = 0);

	VvTSTickSlice* get_tick_slice_by_count(const char* stdCode, uint32_t count, uint64_t etime = 0);
	VvTSKlineSlice* get_kline_slice_by_count(const char* stdCode, VvTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0);

	/*
	 *	获取复权因子
	 *	@stdCode	合约代码
	 *	@commInfo	品种信息
	 */
	double	get_exright_factor(const char* stdCode, VvTSCommodityInfo* commInfo = NULL);

	void	subscribe_bar(const char* stdCode, VvTSKlinePeriod period, uint32_t times);
	void	clear_subbed_bars();
	void	update_bars(const char* stdCode, VvTSTickData* newTick);

	void	clear_cache();

private:
	IRdmDtReader*			_reader;
	FuncDeleteRdmDtReader	_remover;

	IBaseDataMgr*	_bd_mgr;
	IHotMgr*		_hot_mgr;
	VvtDtRunner*		_runner;
	bool			_align_by_section;

	//K线缓存
	typedef struct _BarCache
	{
		VvTSKlineData*	_bars;
		uint64_t		_last_bartime;
		VvTSKlinePeriod	_period;
		uint32_t		_times;

		_BarCache():_last_bartime(0),_period(KP_DAY),_times(1),_bars(NULL){}
	} BarCache;
	typedef vvt_hashmap<std::string, BarCache>	BarCacheMap;
	BarCacheMap	_bars_cache;

	typedef VvTSHashMap<std::string>	RtBarMap;
	RtBarMap*		_rt_bars;
	StdUniqueMutex	_mtx_rtbars;
};

NS_VVTP_END