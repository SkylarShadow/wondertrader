#include "IndexFactory.h"
#include "DataManager.h"
#include "../Includes/VvTSVariant.hpp"
#include "../Includes/VvTSDataDef.hpp"
#include "../Includes/VvTSContractInfo.hpp"
#include "../Share/StrUtil.hpp"

bool IndexFactory::init(VvTSVariant* config, IHotMgr* hotMgr, IBaseDataMgr* bdMgr, DataManager* dataMgr)
{
	_hot_mgr = hotMgr;
	_bd_mgr = bdMgr;
	_data_mgr = dataMgr;

	uint32_t poolsize = config->getUInt32("poolsize");
	if (poolsize > 0)
	{
		_pool.reset(new boost::threadpool::pool(poolsize));
	}

	VvTSVariant* cfgIdx = config->get("indice");
	if(cfgIdx == NULL || !cfgIdx->isArray())
	{
		return false;
	}

	auto cnt = cfgIdx->size();
	for(std::size_t i = 0; i < cnt; i++)
	{
		VvTSVariant* cfgItem = cfgIdx->get(i);
		if(!cfgItem->getBoolean("active"))
			continue;

		IndexWorkerPtr worker(new IndexWorker(this));
		if (!worker->init(cfgItem))
			continue;

		_workers.emplace_back(worker);
	}

	return true;
}

void IndexFactory::handle_quote(VvTSTickData* newTick)
{
	if (newTick == NULL)
		return;

	const char* fullCode = newTick->getContractInfo()->getFullCode();
	auto it = _subbed.find(fullCode);
	if (it == _subbed.end())
		return;

	newTick->retain();

	if(_pool)
	{	
		_pool->schedule([this, newTick, fullCode]() {

			for(IndexWorkerPtr& worker : _workers)
			{
				worker->handle_quote(newTick);
			}
			//这里加一个处理
			newTick->release();
		});
	}
	else
	{
		for (IndexWorkerPtr& worker : _workers)
		{
			worker->handle_quote(newTick);
		}
	}
}

void IndexFactory::push_tick(VvTSTickData* newTick)
{
	_data_mgr->writeTick(newTick, 1);
}

VvTSTickData* IndexFactory::sub_ticks(const char* fullCode)
{
	_subbed.insert(fullCode);
	
	auto ay = StrUtil::split(fullCode, ".");
	return _data_mgr->getCurTick(ay[1].c_str(), ay[0].c_str());
}