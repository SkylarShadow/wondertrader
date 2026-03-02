#pragma once
#include "DataDefineAD.h"

#include "../VvTSUtils/VvtLMDB.hpp"

#include "../Includes/FasterDefs.h"
#include "../Includes/IDataWriter.h"
#include "../Share/StdUtils.hpp"
#include "../Share/BoostMappingFile.hpp"

#include <queue>

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

NS_VVTP_BEGIN
class VvTSContractInfo;
NS_VVTP_END

USING_NS_VVTP;

class WtDataWriterAD : public IDataWriter
{
public:
	WtDataWriterAD();
	~WtDataWriterAD();	

private:
	template<typename HeaderType, typename T>
	void* resizeRTBlock(BoostMFPtr& mfPtr, uint32_t nCount);

public:
	virtual bool init(VvTSVariant* params, IDataWriterSink* sink) override;
	virtual void release() override;

	virtual bool writeTick(VvTSTickData* curTick, uint32_t procFlag) override;

	virtual VvTSTickData* getCurTick(const char* code, const char* exchg = "") override;

private:
	IBaseDataMgr*		_bd_mgr;

	//Tick缓存
	StdUniqueMutex	_mtx_tick_cache;
	std::string		_cache_file_tick;
	vvt_hashmap<std::string, uint32_t> _tick_cache_idx;
	BoostMFPtr		_tick_cache_file;
	RTTickCache*	_tick_cache_block;

	//m1缓存
	typedef struct _RTBarCacheWrapper
	{
		StdUniqueMutex	_mtx;
		std::string		_filename;
		vvt_hashmap<std::string, uint32_t> _idx;
		BoostMFPtr		_file_ptr;
		RTBarCache*		_cache_block;

		_RTBarCacheWrapper():_cache_block(NULL),_file_ptr(NULL){}

		inline bool empty() const { return _cache_block == NULL; }
	} RTBarCacheWrapper;

	RTBarCacheWrapper _m1_cache;
	RTBarCacheWrapper _m5_cache;
	RTBarCacheWrapper _d1_cache;

	typedef std::function<void()> TaskInfo;
	std::queue<TaskInfo>	_tasks;
	StdThreadPtr			_task_thrd;
	StdUniqueMutex			_task_mtx;
	StdCondVariable			_task_cond;
	bool					_async_task;

	std::string		_base_dir;
	uint32_t		_log_group_size;

	bool			_terminated;

	bool			_disable_tick;
	bool			_disable_min1;
	bool			_disable_min5;
	bool			_disable_day;

	uint32_t		_tick_mapsize;
	uint32_t		_kline_mapsize;

private:
	//////////////////////////////////////////////////////////////////////////
	/*
	 *	这里放LMDB的数据库定义
	 *	K线数据，按照每个市场m1/m5/d1三个周期一共三个数据库，路径如./m1/CFFEX
	 *	Tick数据，每个合约一个数据库，路径如./ticks/CFFEX/IF2101
	 */
	typedef std::shared_ptr<VvtLMDB> VvtLMDBPtr;
	typedef vvt_hashmap<std::string, VvtLMDBPtr> VvtLMDBMap;

	VvtLMDBMap	_exchg_m1_dbs;
	VvtLMDBMap	_exchg_m5_dbs;
	VvtLMDBMap	_exchg_d1_dbs;

	//用exchg.code作为key，如BINANCE.BTCUSDT
	VvtLMDBMap	_tick_dbs;

	VvtLMDBPtr	get_k_db(const char* exchg, VvTSKlinePeriod period);

	VvtLMDBPtr	get_t_db(const char* exchg, const char* code);

private:
	void loadCache();

	bool updateTickCache(VvTSContractInfo* ct, VvTSTickData* curTick, uint32_t procFlag);

	void updateBarCache(VvTSContractInfo* ct, VvTSTickData* curTick);

	void pipeToTicks(VvTSContractInfo* ct, VvTSTickData* curTick);

	void pipeToDayBars(VvTSContractInfo* ct, const VvTSBarStruct& bar);

	void pipeToM1Bars(VvTSContractInfo* ct, const VvTSBarStruct& bar);

	void pipeToM5Bars(VvTSContractInfo* ct, const VvTSBarStruct& bar);

	void pushTask(TaskInfo task);
};

