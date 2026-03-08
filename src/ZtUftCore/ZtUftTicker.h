/*!
 * \file ZtHftTicker.h
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
class ZTSTickData;

class ZtUftEngine;

class ZtUftRtTicker
{
public:
	ZtUftRtTicker(ZtUftEngine* engine);
	~ZtUftRtTicker();

public:
	void	init(const char* sessionID);
	void	on_tick(ZTSTickData* curTick);

	void	run();
	void	stop();

private:
	ZTSSessionInfo*	_s_info;
	ZtUftEngine*	_engine;

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