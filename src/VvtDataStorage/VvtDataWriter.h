#pragma once
#include "DataDefine.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/IDataWriter.h"
#include "../Share/StdUtils.hpp"
#include "../Share/BoostMappingFile.hpp"
#include "../Share/SpinMutex.hpp"

#include <queue>
#include <map>

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

NS_VVTP_BEGIN
class VvTSObject;
class VvTSContractInfo;
NS_VVTP_END

USING_NS_VVTP;

class VvtDataWriter : public IDataWriter
{
public:
	VvtDataWriter();
	~VvtDataWriter();	

private:
	template<typename HeaderType, typename T>
	void* resizeRTBlock(BoostMFPtr& mfPtr, uint32_t nCount);

	void  proc_loop();

	void  check_loop();

	uint32_t  dump_bars_to_file(VvTSContractInfo* ct);

	uint32_t  dump_bars_via_dumper(VvTSContractInfo* ct);

private:
	bool	dump_day_data(VvTSContractInfo* ct, VvTSBarStruct* newBar);

	bool	proc_block_data(const char* tag, std::string& content, bool isBar, bool bKeepHead = true);

	void	procTick(VvTSTickData* curTick, uint32_t procFlag);
	void	procQueue(VvTSOrdQueData* curOrdQue);
	void	procOrder(VvTSOrdDtlData* curOrdDetail);
	void	procTrans(VvTSTransData* curTrans);

public:
	virtual bool init(VvTSVariant* params, IDataWriterSink* sink) override;
	virtual void release() override;

	virtual bool writeTick(VvTSTickData* curTick, uint32_t procFlag) override;

	virtual bool writeOrderQueue(VvTSOrdQueData* curOrdQue) override;

	virtual bool writeOrderDetail(VvTSOrdDtlData* curOrdDetail) override;

	virtual bool writeTransaction(VvTSTransData* curTrans) override;

	virtual void transHisData(const char* sid) override;
	
	virtual bool isSessionProceeded(const char* sid) override;

	virtual VvTSTickData* getCurTick(const char* code, const char* exchg = "") override;

private:
	IBaseDataMgr*		_bd_mgr;

	typedef struct _KBlockPair
	{
		RTKlineBlock*	_block;
		BoostMFPtr		_file;
		SpinMutex		_mutex;
		uint64_t		_lasttime;

		_KBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_lasttime = 0;
		}

	} KBlockPair;
	typedef vvt_hashmap<std::string, KBlockPair*>	KBlockFilesMap;

	typedef struct _TickBlockPair
	{
		RTTickBlock*	_block;
		BoostMFPtr		_file;
		SpinMutex		_mutex;
		uint64_t		_lasttime;

		std::shared_ptr< std::ofstream>	_fstream;

		_TickBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_fstream = NULL;
			_lasttime = 0;
		}
	} TickBlockPair;
	typedef vvt_hashmap<std::string, TickBlockPair*>	TickBlockFilesMap;

	typedef struct _TransBlockPair
	{
		RTTransBlock*	_block;
		BoostMFPtr		_file;
		SpinMutex		_mutex;
		uint64_t		_lasttime;

		_TransBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_lasttime = 0;
		}
	} TransBlockPair;
	typedef vvt_hashmap<std::string, TransBlockPair*>	TransBlockFilesMap;

	typedef struct _OdeDtlBlockPair
	{
		RTOrdDtlBlock*	_block;
		BoostMFPtr		_file;
		SpinMutex		_mutex;
		uint64_t		_lasttime;

		_OdeDtlBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_lasttime = 0;
		}
	} OrdDtlBlockPair;
	typedef vvt_hashmap<std::string, OrdDtlBlockPair*>	OrdDtlBlockFilesMap;

	typedef struct _OdeQueBlockPair
	{
		RTOrdQueBlock*	_block;
		BoostMFPtr		_file;
		SpinMutex		_mutex;
		uint64_t		_lasttime;

		_OdeQueBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_lasttime = 0;
		}
	} OrdQueBlockPair;
	typedef vvt_hashmap<std::string, OrdQueBlockPair*>	OrdQueBlockFilesMap;
	

	KBlockFilesMap	_rt_min1_blocks;
	KBlockFilesMap	_rt_min5_blocks;

	TickBlockFilesMap	_rt_ticks_blocks;
	TransBlockFilesMap	_rt_trans_blocks;
	OrdDtlBlockFilesMap _rt_orddtl_blocks;
	OrdQueBlockFilesMap _rt_ordque_blocks;

	SpinMutex		_lck_tick_cache;
	vvt_hashmap<std::string, uint32_t> _tick_cache_idx;
	BoostMFPtr		_tick_cache_file;
	RTTickCache*	_tick_cache_block;

	//typedef std::function<void()> TaskInfo;
	typedef struct alignas(64) _TaskInfo
	{
		VvTSObject*	_obj;
		uint64_t	_type;
		uint32_t	_flag;		

		_TaskInfo(VvTSObject* data, uint64_t dtype, uint32_t flag = 0);

		_TaskInfo(const _TaskInfo& rhs);

		~_TaskInfo();

	} TaskInfo;
	std::queue<TaskInfo>	_tasks;
	StdThreadPtr			_task_thrd;
	StdUniqueMutex			_task_mtx;
	StdCondVariable			_task_cond;

	std::string		_base_dir;
	std::string		_cache_file;
	uint32_t		_log_group_size;
	bool			_async_proc;

	StdCondVariable	_proc_cond;
	StdUniqueMutex	_proc_mtx;
	std::queue<std::string> _proc_que;
	StdThreadPtr	_proc_thrd;
	StdThreadPtr	_proc_chk;
	bool			_terminated;

	bool			_save_tick_log;
	bool			_skip_notrade_tick;
	bool			_skip_notrade_bar;
	bool			_disable_his;

	bool			_disable_tick;
	bool			_disable_min1;
	bool			_disable_min5;
	bool			_disable_day;

	bool			_disable_trans;
	bool			_disable_ordque;
	bool			_disable_orddtl;

	/*
	 *	by Wesley @ 2023.05.04
	 *	分钟线价格模式，0-常规模式，1-将买卖价也记录下来，这个设计时只针对期权这种不活跃的品种
	 */
	uint32_t		_min_price_mode;
	
	std::map<std::string, uint32_t> _proc_date;

private:
	void loadCache();

	bool updateCache(VvTSContractInfo* ct, VvTSTickData* curTick, uint32_t procFlag);

	void pipeToTicks(VvTSContractInfo* ct, VvTSTickData* curTick);

	void pipeToKlines(VvTSContractInfo* ct, VvTSTickData* curTick);

	KBlockPair* getKlineBlock(VvTSContractInfo* ct, VvTSKlinePeriod period, bool bAutoCreate = true);

	TickBlockPair* getTickBlock(VvTSContractInfo* ct, uint32_t curDate, bool bAutoCreate = true);
	TransBlockPair* getTransBlock(VvTSContractInfo* ct, uint32_t curDate, bool bAutoCreate = true);
	OrdDtlBlockPair* getOrdDtlBlock(VvTSContractInfo* ct, uint32_t curDate, bool bAutoCreate = true);
	OrdQueBlockPair* getOrdQueBlock(VvTSContractInfo* ct, uint32_t curDate, bool bAutoCreate = true);

	template<typename T>
	void	releaseBlock(T* block);

	void pushTask(const TaskInfo& task);
};

