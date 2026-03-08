#include "HisDataMgr.h"
#include "ZtHelper.h"
#include "../Share/DLLHelper.hpp"
#include "../Includes/ZTSVariant.hpp"
#include "../ZTSTools/ZTSLogger.h"

void HisDataMgr::reader_log(ZTSLogLevel ll, const char* message)
{
	ZTSLogger::log_raw(ll, message);
}

bool HisDataMgr::init(ZTSVariant* cfg)
{
	std::string module = cfg->getCString("module");
	if (module.empty())
		module = ZtHelper::getInstDir() + DLLHelper::wrap_module("ZtDataStorage");
	else
		module = ZtHelper::getInstDir() + DLLHelper::wrap_module(module.c_str());

	DllHandle libParser = DLLHelper::load_library(module.c_str());
	if (libParser)
	{
		FuncCreateBtDtReader pFuncCreator = (FuncCreateBtDtReader)DLLHelper::get_symbol(libParser, "createBtDtReader");
		if (pFuncCreator == NULL)
		{
			ZTSLogger::error("Initializing of backtest data reader failed: function createBtDtReader not found...");
		}

		if (pFuncCreator)
		{
			_reader = pFuncCreator();
		}

		ZTSLogger::debug("Back data storage module {} loaded", module);
	}
	else
	{
		ZTSLogger::error("Loading module back data storage module {} failed", module);

	}

	_reader->init(cfg, this);

	return true;
}

bool HisDataMgr::load_raw_bars(const char* exchg, const char* code, ZTSKlinePeriod period, FuncLoadDataCallback cb)
{
	if(_reader == NULL)
	{
		ZTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
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
		ZTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
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
		ZTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
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
		ZTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
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
		ZTSLogger::log_raw(LL_ERROR, "Backtest Data Reader not initialized");
		return false;
	}

	std::string buffer;
	bool bSucc = _reader->read_raw_order_details(exchg, code, uDate, buffer);
	if (bSucc)
		cb(buffer);
	return bSucc;
}