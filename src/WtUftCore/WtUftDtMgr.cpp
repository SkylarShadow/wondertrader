/*!
 * \file WtDataManager.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtUftDtMgr.h"
#include "WtUftEngine.h"
#include "WtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Includes/VvTSDataDef.hpp"
#include "../Includes/VvTSVariant.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSDataFactory.h"


WTSDataFactory g_dataFact;

WtUftDtMgr::WtUftDtMgr()
	: _engine(NULL)
	, _bars_cache(NULL)
	, _ticks_cache(NULL)
	, _rt_tick_map(NULL)
{
}


WtUftDtMgr::~WtUftDtMgr()
{
	if (_bars_cache)
		_bars_cache->release();

	if (_ticks_cache)
		_ticks_cache->release();

	if (_rt_tick_map)
		_rt_tick_map->release();
}

bool WtUftDtMgr::init(VvTSVariant* cfg, WtUftEngine* engine)
{
	_engine = engine;

	return true;
}

void WtUftDtMgr::handle_push_quote(const char* stdCode, VvTSTickData* newTick)
{
	if (newTick == NULL)
		return;

	if (_rt_tick_map == NULL)
		_rt_tick_map = DataCacheMap::create();

	_rt_tick_map->add(stdCode, newTick, true);

	if(_ticks_cache != NULL)
	{
		VvTSHisTickData* tData = (VvTSHisTickData*)_ticks_cache->get(stdCode);
		if (tData == NULL)
			return;

		if (tData->isValidOnly() && newTick->volume() == 0)
			return;

		tData->appendTick(newTick->getTickStruct());
	}
}

VvTSTickData* WtUftDtMgr::grab_last_tick(const char* code)
{
	if (_rt_tick_map == NULL)
		return NULL;

	VvTSTickData* curTick = (VvTSTickData*)_rt_tick_map->grab(code);
	if (curTick == NULL)
		return NULL;

	return curTick;
}


VvTSTickSlice* WtUftDtMgr::get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}

VvTSOrdQueSlice* WtUftDtMgr::get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}

VvTSOrdDtlSlice* WtUftDtMgr::get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}

VvTSTransSlice* WtUftDtMgr::get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}

VvTSKlineSlice* WtUftDtMgr::get_kline_slice(const char* stdCode, VvTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime /* = 0 */)
{
	return NULL;
}
