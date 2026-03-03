/*!
 * \file WtHftEngine.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#define WIN32_LEAN_AND_MEAN

#include "WtUftEngine.h"
#include "WtUftTicker.h"
#include "WtUftDtMgr.h"
#include "TraderAdapter.h"
#include "VvtHelper.h"

#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"

#include "../Includes/VvTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/VvTSContractInfo.hpp"

#include "../VvTSTools/VvTSLogger.h"

USING_NS_VVTP;

WtUftEngine::WtUftEngine()
	: _cfg(NULL)
	, _tm_ticker(NULL)
	, _notifier(NULL)
{
	TimeUtils::getDateTime(_cur_date, _cur_time);
	_cur_secs = _cur_time % 100000;
	_cur_time /= 100000;
	_cur_raw_time = _cur_time;
	_cur_tdate = _cur_date;

	VvtHelper::setTime(_cur_date, _cur_time, _cur_secs);
}


WtUftEngine::~WtUftEngine()
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

void WtUftEngine::set_date_time(uint32_t curDate, uint32_t curTime, uint32_t curSecs /* = 0 */, uint32_t rawTime /* = 0 */)
{
	_cur_date = curDate;
	_cur_time = curTime;
	_cur_secs = curSecs;

	if (rawTime == 0)
		rawTime = curTime;

	_cur_raw_time = rawTime;

	VvtHelper::setTime(_cur_date, _cur_raw_time, _cur_secs);
}

void WtUftEngine::set_trading_date(uint32_t curTDate)
{
	_cur_tdate = curTDate;

	VvtHelper::setTDate(curTDate);
}

VvTSCommodityInfo* WtUftEngine::get_commodity_info(const char* stdCode)
{
	const StringVector& ay = StrUtil::split(stdCode, ".");
	VvTSContractInfo* cInfo = _base_data_mgr->getContract(ay[1].c_str(), ay[0].c_str());
	if (cInfo == NULL)
		return NULL;

	return cInfo->getCommInfo();
}

VvTSContractInfo* WtUftEngine::get_contract_info(const char* stdCode)
{
	const StringVector& ay = StrUtil::split(stdCode, ".");
	return _base_data_mgr->getContract(ay[1].c_str(), ay[0].c_str());
}

VvTSSessionInfo* WtUftEngine::get_session_info(const char* sid, bool isCode /* = false */)
{
	if (!isCode)
		return _base_data_mgr->getSession(sid);

	const StringVector& ay = StrUtil::split(sid, ".");
	VvTSContractInfo* cInfo = _base_data_mgr->getContract(ay[1].c_str(), ay[0].c_str());
	if (cInfo == NULL)
		return NULL;

	VvTSCommodityInfo* commInfo = cInfo->getCommInfo();
	return commInfo->getSessionInfo();
}

VvTSTickSlice* WtUftEngine::get_tick_slice(uint32_t sid, const char* code, uint32_t count)
{
	return NULL;
	return _data_mgr->get_tick_slice(code, count);
}

VvTSTickData* WtUftEngine::get_last_tick(uint32_t sid, const char* stdCode)
{
	return _data_mgr->grab_last_tick(stdCode);
}

VvTSKlineSlice* WtUftEngine::get_kline_slice(uint32_t sid, const char* stdCode, const char* period, uint32_t count, uint32_t times /* = 1 */, uint64_t etime /* = 0 */)
{
	return NULL;
	VvTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(stdCode);
	if (cInfo == NULL)
		return NULL;

	VvTSSessionInfo* sInfo = cInfo->getSessionInfo();

	std::string key = fmt::format("{}-{}-{}", stdCode, period, times);
	SubList& sids = _bar_sub_map[key];
	sids.insert(sid);

	VvTSKlinePeriod kp;
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

void WtUftEngine::sub_tick(uint32_t sid, const char* stdCode)
{
	SubList& sids = _tick_sub_map[stdCode];
	sids.insert(sid);
}

double WtUftEngine::get_cur_price(const char* stdCode)
{
	VvTSTickData* lastTick = _data_mgr->grab_last_tick(stdCode);
	if (lastTick == NULL)
		return 0.0;

	double ret = lastTick->price();
	lastTick->release();
	return ret;
}

void WtUftEngine::notify_params_update(const char* name)
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

void WtUftEngine::init(VvTSVariant* cfg, IBaseDataMgr* bdMgr, WtUftDtMgr* dataMgr, EventNotifier* notifier)
{
	_base_data_mgr = bdMgr;
	_data_mgr = dataMgr;
	_notifier = notifier;

	_cfg = cfg;
	if(_cfg) _cfg->retain();
}

void WtUftEngine::run()
{
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		UftContextPtr& ctx = (UftContextPtr&)it->second;
		ctx->on_init();
	}

	_tm_ticker = new WtUftRtTicker(this);
	if(_cfg && _cfg->has("product"))
	{
		VvTSVariant* cfgProd = _cfg->get("product");
		_tm_ticker->init(cfgProd->getCString("session"));
	}
	else
	{
		_tm_ticker->init("ALLDAY");
	}

	_tm_ticker->run();
}

void WtUftEngine::handle_push_quote(VvTSTickData* newTick)
{
	if (_tm_ticker)
		_tm_ticker->on_tick(newTick);
}

void WtUftEngine::handle_push_order_detail(VvTSOrdDtlData* curOrdDtl)
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

void WtUftEngine::handle_push_order_queue(VvTSOrdQueData* curOrdQue)
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

void WtUftEngine::handle_push_transaction(VvTSTransData* curTrans)
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

void WtUftEngine::sub_order_detail(uint32_t sid, const char* stdCode)
{
	SubList& sids = _orddtl_sub_map[stdCode];
	sids.insert(sid);
}

void WtUftEngine::sub_order_queue(uint32_t sid, const char* stdCode)
{
	SubList& sids = _ordque_sub_map[stdCode];
	sids.insert(sid);
}

void WtUftEngine::sub_transaction(uint32_t sid, const char* stdCode)
{
	SubList& sids = _trans_sub_map[stdCode];
	sids.insert(sid);
}

void WtUftEngine::on_session_begin()
{
	VvTSLogger::info("Trading day {} begun", _cur_tdate);

	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		UftContextPtr& ctx = (UftContextPtr&)it->second;
		ctx->on_session_begin(_cur_tdate);
	}
}

void WtUftEngine::on_session_end()
{
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		UftContextPtr& ctx = (UftContextPtr&)it->second;
		ctx->on_session_end(_cur_tdate);
	}

	VvTSLogger::info("Trading day {} ended", _cur_tdate);
}

void WtUftEngine::on_tick(const char* stdCode, VvTSTickData* curTick)
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

void WtUftEngine::on_bar(const char* stdCode, const char* period, uint32_t times, VvTSBarStruct* newBar)
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

void WtUftEngine::on_minute_end(uint32_t curDate, uint32_t curTime)
{

}

void WtUftEngine::addContext(UftContextPtr ctx)
{
	uint32_t sid = ctx->id();
	_ctx_map[sid] = ctx;
}

UftContextPtr WtUftEngine::getContext(uint32_t id)
{
	auto it = _ctx_map.find(id);
	if (it == _ctx_map.end())
		return UftContextPtr();

	return it->second;
}

VvTSOrdQueSlice* WtUftEngine::get_order_queue_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_order_queue_slice(code, count);
}

VvTSOrdDtlSlice* WtUftEngine::get_order_detail_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_order_detail_slice(code, count);
}

VvTSTransSlice* WtUftEngine::get_transaction_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_transaction_slice(code, count);
}