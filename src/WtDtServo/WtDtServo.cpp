/*!
 * \file WtDtPorter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDtServo.h"
#include "WtDtRunner.h"

#include "../WtDtCore/WtHelper.h"
#include "../WTSTools/WTSLogger.h"

#include "../Share/ModuleHelper.hpp"
#include "../Includes/VvTSVersion.h"
#include "../Includes/VvTSDataDef.hpp"

#include <boost/filesystem.hpp>

#ifdef _MSC_VER
#ifdef _WIN64
char PLATFORM_NAME[] = "X64";
#else
char PLATFORM_NAME[] = "X86";
#endif
#else
char PLATFORM_NAME[] = "UNIX";
#endif

#ifdef _MSC_VER
#include "../Common/mdump.h"
const char* getModuleName()
{
	static char MODULE_NAME[250] = { 0 };
	if (strlen(MODULE_NAME) == 0)
	{
		GetModuleFileName(g_dllModule, MODULE_NAME, 250);
		boost::filesystem::path p(MODULE_NAME);
		strcpy(MODULE_NAME, p.filename().string().c_str());
	}

	return MODULE_NAME;
}
#endif

WtDtRunner& getRunner()
{
	static WtDtRunner runner;
	return runner;
}

void initialize(VvTString cfgFile, bool isFile, VvTString logCfg, FuncOnTickCallback cbTick, FuncOnBarCallback cbBar)
{
	getRunner().initialize(cfgFile, isFile, getBinDir(), logCfg, cbTick, cbBar);
}

const char* get_version()
{
	static std::string _ver;
	if (_ver.empty())
	{
		_ver = PLATFORM_NAME;
		_ver += " ";
		_ver += VVT_VERSION;
		_ver += " Build@";
		_ver += __DATE__;
		_ver += " ";
		_ver += __TIME__;
	}
	return _ver.c_str();
}

VvTUInt32 get_bars_by_range(const char* stdCode, const char* period, VvTUInt64 beginTime, VvTUInt64 endTime, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt)
{
	VvTSKlineSlice* kData = getRunner().get_bars_by_range(stdCode, period, beginTime, endTime);
	if (kData)
	{
		uint32_t reaCnt = kData->size();
		cbCnt(kData->size());

		for (std::size_t i = 0; i < kData->get_block_counts(); i++)
			cb(kData->get_block_addr(i), kData->get_block_size(i), i == kData->get_block_counts() - 1);

		kData->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

VvTUInt32 get_bars_by_date(const char* stdCode, const char* period, VvTUInt32 uDate, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt)
{
	VvTSKlineSlice* kData = getRunner().get_bars_by_date(stdCode, period, uDate);
	if (kData)
	{
		uint32_t reaCnt = kData->size();
		cbCnt(kData->size());

		for (std::size_t i = 0; i < kData->get_block_counts(); i++)
			cb(kData->get_block_addr(i), kData->get_block_size(i), i == kData->get_block_counts() - 1);

		kData->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

VvTUInt32	get_ticks_by_range(const char* stdCode, VvTUInt64 beginTime, VvTUInt64 endTime, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt)
{
	VvTSTickSlice* slice = getRunner().get_ticks_by_range(stdCode, beginTime, endTime);
	if (slice)
	{
		uint32_t reaCnt = 0;
		uint32_t blkCnt = slice->get_block_counts();
		cbCnt(slice->size());

		for(uint32_t sIdx = 0; sIdx < blkCnt; sIdx++)
		{
			cb(slice->get_block_addr(sIdx), slice->get_block_size(sIdx), sIdx == blkCnt - 1);
			reaCnt += slice->get_block_size(sIdx);
		}
		
		slice->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

VvTUInt32 get_bars_by_count(const char* stdCode, const char* period, VvTUInt32 count, VvTUInt64 endTime, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt)
{
	VvTSKlineSlice* kData = getRunner().get_bars_by_count(stdCode, period, count, endTime);
	if (kData)
	{
		uint32_t reaCnt = kData->size();
		cbCnt(kData->size());

		for(std::size_t i = 0; i< kData->get_block_counts(); i++)
			cb(kData->get_block_addr(i), kData->get_block_size(i), i == kData->get_block_counts()-1);

		kData->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

VvTUInt32	get_ticks_by_count(const char* stdCode, VvTUInt32 count, VvTUInt64 endTime, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt)
{
	VvTSTickSlice* slice = getRunner().get_ticks_by_count(stdCode, count, endTime);
	if (slice)
	{
		uint32_t reaCnt = 0;
		uint32_t blkCnt = slice->get_block_counts();
		cbCnt(slice->size());

		for (uint32_t sIdx = 0; sIdx < blkCnt; sIdx++)
		{
			cb(slice->get_block_addr(sIdx), slice->get_block_size(sIdx), sIdx == blkCnt - 1);
			reaCnt += slice->get_block_size(sIdx);
		}

		slice->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

VvTUInt32 get_ticks_by_date(const char* stdCode, VvTUInt32 uDate, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt)
{
	VvTSTickSlice* slice = getRunner().get_ticks_by_date(stdCode, uDate);
	if (slice)
	{
		uint32_t reaCnt = 0;
		uint32_t blkCnt = slice->get_block_counts();
		cbCnt(slice->size());

		for (uint32_t sIdx = 0; sIdx < blkCnt; sIdx++)
		{
			cb(slice->get_block_addr(sIdx), slice->get_block_size(sIdx), sIdx == blkCnt - 1);
			reaCnt += slice->get_block_size(sIdx);
		}

		slice->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

VvTUInt32 get_sbars_by_date(const char* stdCode, VvTUInt32 secs, VvTUInt32 uDate, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt)
{
	VvTSKlineSlice* kData = getRunner().get_sbars_by_date(stdCode, secs, uDate);
	if (kData)
	{
		uint32_t reaCnt = kData->size();
		cbCnt(kData->size());

		for (std::size_t i = 0; i < kData->get_block_counts(); i++)
			cb(kData->get_block_addr(i), kData->get_block_size(i), i == kData->get_block_counts() - 1);

		kData->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

void subscribe_tick(const char* stdCode, bool bReplace)
{
	getRunner().sub_tick(stdCode, bReplace);
}

void subscribe_bar(const char* stdCode, const char* period)
{
	getRunner().sub_bar(stdCode, period);
}

void clear_cache()
{
	getRunner().clear_cache();
}