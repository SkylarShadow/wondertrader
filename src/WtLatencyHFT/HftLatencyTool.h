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
#include "../VvtCore/VvtHftEngine.h"
#include "../VvtCore/HftStrategyMgr.h"
#include "../VvtCore/TraderAdapter.h"
#include "../VvtCore/ParserAdapter.h"
#include "../VvtCore/ActionPolicyMgr.h"
#include "../VvtCore/VvtDtMgr.h"

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

		VvtHftEngine			_engine;

		VvTSBaseDataMgr		_bd_mgr;
		VvTSHotMgr			_hot_mgr;
		ActionPolicyMgr		_act_mgr;
		VvtDtMgr				_dt_mgr;

		uint32_t			_times;
		uint32_t			_core;
	};
}

