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

class WtSelEngine;

class WtSelRtTicker
{
public:
	WtSelRtTicker(WtSelEngine* engine);
	~WtSelRtTicker();

public:
	void	init(IDataReader* store, const char* sessionID);
	void	on_tick(VvTSTickData* curTick, uint32_t hotFlag = 0);

	void	run();
	void	stop();

private:
	void	trigger_price(VvTSTickData* curTick, uint32_t hotFlag = 0);

private:
	VvTSSessionInfo*	_s_info;
	WtSelEngine*	_engine;
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