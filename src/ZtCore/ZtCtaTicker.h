/*!
 * \file ZtCtaTicker.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>
#include <atomic>

#include "../Includes/ZTSMarcos.h"
#include "../Share/StdUtils.hpp"

NS_ZTP_BEGIN
class ZTSSessionInfo;
class IDataReader;
class ZTSTickData;

class ZtCtaEngine;
//////////////////////////////////////////////////////////////////////////
//生产时间步进器
class ZtCtaRtTicker
{
public:
	ZtCtaRtTicker(ZtCtaEngine* engine) 
		: _engine(engine)
		, _stopped(false)
		, _date(0)
		, _time(UINT_MAX)
		, _next_check_time(0)
		, _last_emit_pos(0)
		, _cur_pos(0){}
	~ZtCtaRtTicker(){}

public:
	void	init(IDataReader* store, const char* sessionID);
	//void	set_time(uint32_t uDate, uint32_t uTime);
	void	on_tick(ZTSTickData* curTick);

	void	run();
	void	stop();

	bool		is_in_trading() const;
	uint32_t	time_to_mins(uint32_t uTime) const;

private:
	void	trigger_price(ZTSTickData* curTick);

private:
	ZTSSessionInfo*	_s_info;
	ZtCtaEngine*	_engine;
	IDataReader*	_store;

	uint32_t	_date;
	uint32_t	_time;

	uint32_t	_cur_pos;

	StdUniqueMutex	_mtx;
	std::atomic<uint64_t>	_next_check_time;
	std::atomic<uint32_t>	_last_emit_pos;

	bool			_stopped;
	StdThreadPtr	_thrd;

};
NS_ZTP_END