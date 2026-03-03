/*!
 * \file VvtHftTicker.h
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

#include "../Includes/VvTSMarcos.h"
#include "../Share/StdUtils.hpp"

NS_VVTP_BEGIN
class VvTSSessionInfo;
class VvTSTickData;

class VvtUftEngine;

class VvtUftRtTicker
{
public:
	VvtUftRtTicker(VvtUftEngine* engine);
	~VvtUftRtTicker();

public:
	void	init(const char* sessionID);
	void	on_tick(VvTSTickData* curTick);

	void	run();
	void	stop();

private:
	VvTSSessionInfo*	_s_info;
	VvtUftEngine*	_engine;

	uint32_t	_date;
	uint32_t	_time;

	uint32_t	_cur_pos;

	StdUniqueMutex	_mtx;
	std::atomic<uint64_t>	_next_check_time;
	std::atomic<uint32_t>	_last_emit_pos;

	bool			_stopped;
	StdThreadPtr	_thrd;
};

NS_VVTP_END