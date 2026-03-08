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
#include "../ZtUftCore/ZtUftEngine.h"
#include "../ZtUftCore/UftStrategyMgr.h"
#include "../ZtUftCore/TraderAdapter.h"
#include "../ZtUftCore/ParserAdapter.h"

#include "../ZTSTools/ZTSBaseDataMgr.h"

NS_ZTP_BEGIN
class ZTSVariant;
NS_ZTP_END

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

		bool initEngine(ZTSVariant* cfg);

	private:
		TraderAdapterMgr	_traders;
		ParserAdapterMgr	_parsers;
		UftStrategyMgr		_stra_mgr;

		ZtUftEngine			_engine;

		ZTSBaseDataMgr		_bd_mgr;

		uint32_t			_times;
		uint32_t			_core;
	};
}

