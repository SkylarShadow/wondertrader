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
class IDataReader;
class ZTSTickData;

class ZtSelEngine;

class ZtSelRtTicker
{
public:
	ZtSelRtTicker(ZtSelEngine* engine);
	~ZtSelRtTicker();

public:
	void	init(IDataReader* store, const char* sessionID);
	void	on_tick(ZTSTickData* curTick, uint32_t hotFlag = 0);

	void	run();
	void	stop();

private:
	void	trigger_price(ZTSTickData* curTick, uint32_t hotFlag = 0);

private:
	ZTSSessionInfo*	_s_info;
	ZtSelEngine*	_engine;
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

NS_ZTP_END