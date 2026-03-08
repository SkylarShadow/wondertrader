/*!
 * \file ZtHftEngine.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#define WIN32_LEAN_AND_MEAN

#include "ZtUftEngine.h"
#include "ZtUftTicker.h"
#include "ZtUftDtMgr.h"
#include "TraderAdapter.h"
#include "ZtHelper.h"

#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"

#include "../Includes/ZTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/ZTSContractInfo.hpp"

#include "../ZTSTools/ZTSLogger.h"

USING_NS_ZTP;

ZtUftEngine::ZtUftEngine()
	: _cfg(NULL)
	, _tm_ticker(NULL)
	, _notifier(NULL)
{
	TimeUtils::getDateTime(_cur_date, _cur_time);
	_cur_secs = _cur_time % 100000;
	_cur_time /= 100000;
	_cur_raw_time = _cur_time;
	_cur_tdate = _cur_date;

	ZtHelper::setTime(_cur_date, _cur_time, _cur_secs);
}


ZtUftEngine::~ZtUftEngine()
{
	if (_tm_ticker)
	{
		_tm_ticker->stop();
		delete _tm_ticker;
		_tm_ticker = NULL;
	}

	if (_cfg)
		_cfg->release();
}

void ZtUftEngine::set_date_time(uint32_t curDate, uint32_t curTime, uint32_t curSecs /* = 0 */, uint32_t rawTime /* = 0 */)
{
	_cur_date = curDate;
	_cur_time = curTime;
	_cur_secs = curSecs;

	if (rawTime == 0)
		rawTime = curTime;

	_cur_raw_time = rawTime;

	ZtHelper::setTime(_cur_date, _cur_raw_time, _cur_secs);
}

void ZtUftEngine::set_trading_date(uint32_t curTDate)
{
	_cur_tdate = curTDate;

	ZtHelper::setTDate(curTDate);
}

ZTSCommodityInfo* ZtUftEngine::get_commodity_info(const char* stdCode)
{
	const StringVector& ay = StrUtil::split(stdCode, ".");
	ZTSContractInfo* cInfo = _base_data_mgr->getContract(ay[1].c_str(), ay[0].c_str());
	if (cInfo == NULL)
		return NULL;

	return cInfo->getCommInfo();
}

ZTSContractInfo* ZtUftEngine::get_contract_info(const char* stdCode)
{
	const StringVector& ay = StrUtil::split(stdCode, ".");
	return _base_data_mgr->getContract(ay[1].c_str(), ay[0].c_str());
}

ZTSSessionInfo* ZtUftEngine::get_session_info(const char* sid, bool isCode /* = false */)
{
	if (!isCode)
		return _base_data_mgr->getSession(sid);

	const StringVector& ay = StrUtil::split(sid, ".");
	ZTSContractInfo* cInfo = _base_data_mgr->getContract(ay[1].c_str(), ay[0].c_str());
	if (cInfo == NULL)
		return NULL;

	ZTSCommodityInfo* commInfo = cInfo->getCommInfo();
	return commInfo->getSessionInfo();
}

ZTSTickSlice* ZtUftEngine::get_tick_slice(uint32_t sid, const char* code, uint32_t count)
{
	return NULL;
	return _data_mgr->get_tick_slice(code, count);
}

ZTSTickData* ZtUftEngine::get_last_tick(uint32_t sid, const char* stdCode)
{
	return _data_mgr->grab_last_tick(stdCode);
}

ZTSKlineSlice* ZtUftEngine::get_kline_slice(uint32_t sid, const char* stdCode, const char* period, uint32_t count, uint32_t times /* = 1 */, uint64_t etime /* = 0 */)
{
	return NULL;
	ZTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(stdCode);
	if (cInfo == NULL)
		return NULL;

	ZTSSessionInfo* sInfo = cInfo->getSessionInfo();

	std::string key = fmt::format("{}-{}-{}", stdCode, period, times);
	SubList& sids = _bar_sub_map[key];
	sids.insert(sid);

	ZTSKlinePeriod kp;
	if (strcmp(period, "m") == 0)
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			times /= 5;
		}
		else
			kp = KP_Minute1;
	}
	else
	{
		kp = KP_DAY;
	}

	return _data_mgr->get_kline_slice(stdCode, kp, times, count, etime);
}

void ZtUftEngine::sub_tick(uint32_t sid, const char* stdCode)
{
	SubList& sids = _tick_sub_map[stdCode];
	sids.insert(sid);
}

double ZtUftEngine::get_cur_price(const char* stdCode)
{
	ZTSTickData* lastTick = _data_mgr->grab_last_tick(stdCode);
	if (lastTick == NULL)
		return 0.0;

	double ret = lastTick->price();
	lastTick->release();
	return ret;
}

void ZtUftEngine::notify_params_update(const char* name)
{
	for(auto& v : _ctx_map)
	{
		const UftContextPtr& context = v.second;
		if(strcmp(context->name(), name) == 0)
		{
			context->on_params_updated();
			break;
		}
	}
}

void ZtUftEngine::init(ZTSVariant* cfg, IBaseDataMgr* bdMgr, ZtUftDtMgr* dataMgr, EventNotifier* notifier)
{
	_base_data_mgr = bdMgr;
	_data_mgr = dataMgr;
	_notifier = notifier;

	_cfg = cfg;
	if(_cfg) _cfg->retain();
}

void ZtUftEngine::run()
{
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		UftContextPtr& ctx = (UftContextPtr&)it->second;
		ctx->on_init();
	}

	_tm_ticker = new ZtUftRtTicker(this);
	if(_cfg && _cfg->has("product"))
	{
		ZTSVariant* cfgProd = _cfg->get("product");
		_tm_ticker->init(cfgProd->getCString("session"));
	}
	else
	{
		_tm_ticker->init("ALLDAY");
	}

	_tm_ticker->run();
}

void ZtUftEngine::handle_push_quote(ZTSTickData* newTick)
{
	if (_tm_ticker)
		_tm_ticker->on_tick(newTick);
}

void ZtUftEngine::handle_push_order_detail(ZTSOrdDtlData* curOrdDtl)
{
	const char* stdCode = curOrdDtl->code();
	auto sit = _orddtl_sub_map.find(stdCode);
	if (sit != _orddtl_sub_map.end())
	{
		const SubList& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			//By Wesley @ 2022.02.07
			//Level2数据一般用于HFT场景，所以不做复权处理
			//所以不读取订阅标记
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				UftContextPtr& ctx = (UftContextPtr&)cit->second;
				ctx->on_order_detail(stdCode, curOrdDtl);
			}
		}
	}
}

void ZtUftEngine::handle_push_order_queue(ZTSOrdQueData* curOrdQue)
{
	const char* stdCode = curOrdQue->code();
	auto sit = _ordque_sub_map.find(stdCode);
	if (sit != _ordque_sub_map.end())
	{
		const SubList& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			//By Wesley @ 2022.02.07
			//Level2数据一般用于HFT场景，所以不做复权处理
			//所以不读取订阅标记
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				UftContextPtr& ctx = (UftContextPtr&)cit->second;
				ctx->on_order_queue(stdCode, curOrdQue);
			}
		}
	}
}

void ZtUftEngine::handle_push_transaction(ZTSTransData* curTrans)
{
	const char* stdCode = curTrans->code();
	auto sit = _trans_sub_map.find(stdCode);
	if (sit != _trans_sub_map.end())
	{
		const SubList& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			//By Wesley @ 2022.02.07
			//Level2数据一般用于HFT场景，所以不做复权处理
			//所以不读取订阅标记
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				UftContextPtr& ctx = (UftContextPtr&)cit->second;
				ctx->on_transaction(stdCode, curTrans);
			}
		}
	}
}

void ZtUftEngine::sub_order_detail(uint32_t sid, const char* stdCode)
{
	SubList& sids = _orddtl_sub_map[stdCode];
	sids.insert(sid);
}

void ZtUftEngine::sub_order_queue(uint32_t sid, const char* stdCode)
{
	SubList& sids = _ordque_sub_map[stdCode];
	sids.insert(sid);
}

void ZtUftEngine::sub_transaction(uint32_t sid, const char* stdCode)
{
	SubList& sids = _trans_sub_map[stdCode];
	sids.insert(sid);
}

void ZtUftEngine::on_session_begin()
{
	ZTSLogger::info("Trading day {} begun", _cur_tdate);

	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		UftContextPtr& ctx = (UftContextPtr&)it->second;
		ctx->on_session_begin(_cur_tdate);
	}
}

void ZtUftEngine::on_session_end()
{
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		UftContextPtr& ctx = (UftContextPtr&)it->second;
		ctx->on_session_end(_cur_tdate);
	}

	ZTSLogger::info("Trading day {} ended", _cur_tdate);
}

void ZtUftEngine::on_tick(const char* stdCode, ZTSTickData* curTick)
{
	if(_data_mgr)
		_data_mgr->handle_push_quote(stdCode, curTick);

	{
		auto sit = _tick_sub_map.find(stdCode);
		if (sit != _tick_sub_map.end())
		{
			const SubList& sids = sit->second;
			for (auto it = sids.begin(); it != sids.end(); it++)
			{
				uint32_t sid = *it;

				auto cit = _ctx_map.find(sid);
				if (cit != _ctx_map.end())
				{
					UftContextPtr& ctx = (UftContextPtr&)cit->second;
					ctx->on_tick(stdCode, curTick);
				}
			}
		}
	}
}

void ZtUftEngine::on_bar(const char* stdCode, const char* period, uint32_t times, ZTSBarStruct* newBar)
{
	std::string key = fmt::format("{}-{}-{}", stdCode, period, times);
	const SubList& sids = _bar_sub_map[key];
	for (auto it = sids.begin(); it != sids.end(); it++)
	{
		uint32_t sid = *it;
		auto cit = _ctx_map.find(sid);
		if (cit != _ctx_map.end())
		{
			UftContextPtr& ctx = (UftContextPtr&)cit->second;
			ctx->on_bar(stdCode, period, times, newBar);
		}
	}
}

void ZtUftEngine::on_minute_end(uint32_t curDate, uint32_t curTime)
{

}

void ZtUftEngine::addContext(UftContextPtr ctx)
{
	uint32_t sid = ctx->id();
	_ctx_map[sid] = ctx;
}

UftContextPtr ZtUftEngine::getContext(uint32_t id)
{
	auto it = _ctx_map.find(id);
	if (it == _ctx_map.end())
		return UftContextPtr();

	return it->second;
}

ZTSOrdQueSlice* ZtUftEngine::get_order_queue_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_order_queue_slice(code, count);
}

ZTSOrdDtlSlice* ZtUftEngine::get_order_detail_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_order_detail_slice(code, count);
}

ZTSTransSlice* ZtUftEngine::get_transaction_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_transaction_slice(code, count);
}