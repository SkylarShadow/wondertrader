#include "HisDataMgr.h"
#include "VvtHelper.h"
#include "../Share/DLLHelper.hpp"
#include "../Includes/VvTSVariant.hpp"
#include "../VvTSTools/VvTSLogger.h"

void HisDataMgr::reader_log(VvTSLogLevel ll, const char* message)
{
	VvTSLogger::log_raw(ll, message);
}

bool HisDataMgr::init(VvTSVariant* cfg)
{
	std::string module = cfg->getCString("module");
	if (module.empty())
		module = VvtHelper::getInstDir() + DLLHelper::wrap_module("WtDataStorage");
	else
		module = VvtHelper::getInstDir() + DLLHelper::wrap_module(module.c_str());

	DllHandle libParser = DLLHelper::load_library(module.c_str());
	if (libParser)
	{
		FuncCreateBtDtReader pFuncCreator = (FuncCreateBtDtReader)DLLHelper::get_symbol(libParser, "createBtDtReader");
		if (pFuncCreator == NULL)
		{
			VvTSLogger::error("Initializing of backtest data reader failed: function createBtDtReader not found...");
		}

		if (pFuncCreator)
		{
			_reader = pFuncCreator();
		}

		VvTSLogger::debug("Back data storage module {} loaded", module);
	}
	else
	{
		VvTSLogger::error("Loading module back data storage module {} failed", module);

	}

	_reader->init(cfg, this);

	return true;
}

bool HisDataMgr::load_raw_bars(const char* exchg, const char* code, VvTSKlinePeriod period, FuncLoadDataCallback cb)
{
	if(_reader == NULL)
	{
		VvTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
		return false;
	}

	std::string buffer;
	bool bSucc = _reader->read_raw_bars(exchg, code, period, buffer);
	if (bSucc)
		cb(buffer);
	return bSucc;
}

bool HisDataMgr::load_raw_ticks(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb)
{
	if (_reader == NULL)
	{
		VvTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
		return false;
	}

	std::string buffer;
	bool bSucc = _reader->read_raw_ticks(exchg, code, uDate, buffer);
	if (bSucc)
		cb(buffer);
	return bSucc;
}

bool HisDataMgr::load_raw_trans(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb)
{
	if (_reader == NULL)
	{
		VvTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
		return false;
	}

	std::string buffer;
	bool bSucc = _reader->read_raw_transactions(exchg, code, uDate, buffer);
	if (bSucc)
		cb(buffer);
	return bSucc;
}

bool HisDataMgr::load_raw_ordque(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb)
{
	if (_reader == NULL)
	{
		VvTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
		return false;
	}

	std::string buffer;
	bool bSucc = _reader->read_raw_order_queues(exchg, code, uDate, buffer);
	if (bSucc)
		cb(buffer);
	return bSucc;
}

bool HisDataMgr::load_raw_orddtl(const char* exchg, const char* code, uint32_t uDate, FuncLoadDataCallback cb)
{
	if (_reader == NULL)
	{
		VvTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
		return false;
	}

	std::string buffer;
	bool bSucc = _reader->read_raw_order_details(exchg, code, uDate, buffer);
	if (bSucc)
		cb(buffer);
	return bSucc;
}