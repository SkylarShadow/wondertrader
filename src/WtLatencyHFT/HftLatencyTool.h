/*!
 * /file WtUftRunner.h
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#pragma once
#include "../WtCore/WtHftEngine.h"
#include "../WtCore/HftStrategyMgr.h"
#include "../WtCore/TraderAdapter.h"
#include "../WtCore/ParserAdapter.h"
#include "../WtCore/ActionPolicyMgr.h"
#include "../WtCore/WtDtMgr.h"

#include "../VvTSTools/VvTSBaseDataMgr.h"
#include "../VvTSTools/VvTSHotMgr.h"


NS_VVTP_BEGIN
class VvTSVariant;
NS_VVTP_END

namespace hft
{
	class HftLatencyTool
	{
	public:
		HftLatencyTool();
		~HftLatencyTool();

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
		HftStrategyMgr		_stra_mgr;

		WtHftEngine			_engine;

		VvTSBaseDataMgr		_bd_mgr;
		VvTSHotMgr			_hot_mgr;
		ActionPolicyMgr		_act_mgr;
		WtDtMgr				_dt_mgr;

		uint32_t			_times;
		uint32_t			_core;
	};
}

