/*!
 * \file WtHftTicker.h
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
class IDataReader;
class VvTSTickData;

class WtHftEngine;

class WtHftRtTicker
{
public:
	WtHftRtTicker(WtHftEngine* engine);
	~WtHftRtTicker();

public:
	void	init(IDataReader* store, const char* sessionID);
	void	on_tick(VvTSTickData* curTick);

	void	run();
	void	stop();

private:
	void	trigger_price(VvTSTickData* curTick);

private:
	VvTSSessionInfo*	_s_info;
	WtHftEngine*	_engine;
	IDataReader*		_store;

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