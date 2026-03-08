/*!
 * /file ZtUftRunner.h
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#pragma once
#include "../ZtCore/ZtHftEngine.h"
#include "../ZtCore/HftStrategyMgr.h"
#include "../ZtCore/TraderAdapter.h"
#include "../ZtCore/ParserAdapter.h"
#include "../ZtCore/ActionPolicyMgr.h"
#include "../ZtCore/ZtDtMgr.h"

#include "../ZTSTools/ZTSBaseDataMgr.h"
#include "../ZTSTools/ZTSHotMgr.h"


NS_ZTP_BEGIN
class ZTSVariant;
NS_ZTP_END

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

		bool initEngine(ZTSVariant* cfg);

	private:
		TraderAdapterMgr	_traders;
		ParserAdapterMgr	_parsers;
		HftStrategyMgr		_stra_mgr;

		ZtHftEngine			_engine;

		ZTSBaseDataMgr		_bd_mgr;
		ZTSHotMgr			_hot_mgr;
		ActionPolicyMgr		_act_mgr;
		ZtDtMgr				_dt_mgr;

		uint32_t			_times;
		uint32_t			_core;
	};
}

