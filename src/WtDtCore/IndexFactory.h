#pragma once
#include "IndexWorker.h"
#include "../Share/threadpool.hpp"
#include <vector>

class DataManager;

class IndexFactory
{
public:
	IndexFactory():_hot_mgr(NULL), _bd_mgr(NULL){}

public:
	bool	init(VvTSVariant* config, IHotMgr* hotMgr, IBaseDataMgr* bdMgr, DataManager* dataMgr);
	void	handle_quote(VvTSTickData* newTick);

public:
	inline IHotMgr*			get_hot_mgr() { return _hot_mgr; }
	inline IBaseDataMgr*	get_bd_mgr() { return _bd_mgr; }

	VvTSTickData*	sub_ticks(const char* fullCode);

	void			push_tick(VvTSTickData* newTick);

private:
	typedef std::vector<IndexWorkerPtr>	IndexWorkers;
	IndexWorkers	_workers;
	IHotMgr*		_hot_mgr;
	IBaseDataMgr*	_bd_mgr;
	DataManager*	_data_mgr;

	typedef std::shared_ptr<boost::threadpool::pool> ThreadPoolPtr;
	ThreadPoolPtr	_pool;

	vvt_hashset<std::string>	_subbed;
};

