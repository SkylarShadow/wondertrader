/*!
 * /file VvtUftRunner.h
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#pragma once
#include "../VvtUftCore/VvtUftEngine.h"
#include "../VvtUftCore/UftStrategyMgr.h"
#include "../VvtUftCore/TraderAdapter.h"
#include "../VvtUftCore/ParserAdapter.h"

#include "../VvTSTools/VvTSBaseDataMgr.h"

NS_VVTP_BEGIN
class VvTSVariant;
NS_VVTP_END

namespace uft
{
	class UftLatencyTool
	{
	public:
		UftLatencyTool();
		~UftLatencyTool();

	public:
		/*
		 *	≥ı ºªØ
		 */
		bool init();

		void run();

	private:
		bool initModules();
		bool initStrategies();

		bool initEngine(VvTSVariant* cfg);

	private:
		TraderAdapterMgr	_traders;
		ParserAdapterMgr	_parsers;
		UftStrategyMgr		_stra_mgr;

		VvtUftEngine			_engine;

		VvTSBaseDataMgr		_bd_mgr;

		uint32_t			_times;
		uint32_t			_core;
	};
}

