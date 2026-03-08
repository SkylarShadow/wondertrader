/*!
 * \file ZtDataManager.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ZtUftDtMgr.h"
#include "ZtUftEngine.h"
#include "ZtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Includes/ZTSDataDef.hpp"
#include "../Includes/ZTSVariant.hpp"

#include "../ZTSTools/ZTSLogger.h"
#include "../ZTSTools/ZTSDataFactory.h"


ZTSDataFactory g_dataFact;

ZtUftDtMgr::ZtUftDtMgr()
	: _engine(NULL)
	, _bars_cache(NULL)
	, _ticks_cache(NULL)
	, _rt_tick_map(NULL)
{
}


ZtUftDtMgr::~ZtUftDtMgr()
{
	if (_bars_cache)
		_bars_cache->release();

	if (_ticks_cache)
		_ticks_cache->release();

	if (_rt_tick_map)
		_rt_tick_map->release();
}

bool ZtUftDtMgr::init(ZTSVariant* cfg, ZtUftEngine* engine)
{
	_engine = engine;

	return true;
}

void ZtUftDtMgr::handle_push_quote(const char* stdCode, ZTSTickData* newTick)
{
	if (newTick == NULL)
		return;

	if (_rt_tick_map == NULL)
		_rt_tick_map = DataCacheMap::create();

	_rt_tick_map->add(stdCode, newTick, true);

	if(_ticks_cache != NULL)
	{
		ZTSHisTickData* tData = (ZTSHisTickData*)_ticks_cache->get(stdCode);
		if (tData == NULL)
			return;

		if (tData->isValidOnly() && newTick->volume() == 0)
			return;

		tData->appendTick(newTick->getTickStruct());
	}
}

ZTSTickData* ZtUftDtMgr::grab_last_tick(const char* code)
{
	if (_rt_tick_map == NULL)
		return NULL;

	ZTSTickData* curTick = (ZTSTickData*)_rt_tick_map->grab(code);
	if (curTick == NULL)
		return NULL;

	return curTick;
}


ZTSTickSlice* ZtUftDtMgr::get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}

ZTSOrdQueSlice* ZtUftDtMgr::get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}

ZTSOrdDtlSlice* ZtUftDtMgr::get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}

ZTSTransSlice* ZtUftDtMgr::get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}

ZTSKlineSlice* ZtUftDtMgr::get_kline_slice(const char* stdCode, ZTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}
