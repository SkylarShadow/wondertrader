/*!
 * \file ZtDtPorter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ZtDtHelper.h"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/BoostFile.hpp"

#include "../ZtDataStorage/DataDefine.h"
#include "../ZTSUtils/ZTSCmpHelper.hpp"
#include "../ZTSTools/CsvHelper.h"
#include "../ZTSTools/ZTSDataFactory.h"

#include "../Includes/ZTSDataDef.hpp"
#include "../Includes/ZTSSessionInfo.hpp"

#include <rapidjson/document.h>

namespace rj = rapidjson;

USING_NS_ZTP;

/*
 *	处理块数据
 */
bool proc_block_data(std::string& content, bool isBar, bool bKeepHead /* = true */)
{
	BlockHeader* header = (BlockHeader*)content.data();

	bool bCmped = header->is_compressed();
	bool bOldVer = header->is_old_version();

	//如果既没有压缩，也不是老版本结构体，则直接返回
	if (!bCmped && !bOldVer)
	{
		if (!bKeepHead)
			content.erase(0, BLOCK_HEADER_SIZE);
		return true;
	}

	std::string buffer;
	if (bCmped)
	{
		BlockHeaderV2* blkV2 = (BlockHeaderV2*)content.c_str();

		if (content.size() != (sizeof(BlockHeaderV2) + blkV2->_size))
		{
			return false;
		}

		//将文件头后面的数据进行解压
		buffer = ZTSCmpHelper::uncompress_data(content.data() + BLOCK_HEADERV2_SIZE, blkV2->_size);
	}
	else
	{
		if (!bOldVer)
		{
			//如果不是老版本，直接返回
			if (!bKeepHead)
				content.erase(0, BLOCK_HEADER_SIZE);
			return true;
		}
		else
		{
			buffer.append(content.data() + BLOCK_HEADER_SIZE, content.size() - BLOCK_HEADER_SIZE);
		}
	}

	if (bOldVer)
	{
		if (isBar)
		{
			std::string bufV2;
			uint32_t barcnt = buffer.size() / sizeof(ZTSBarStructOld);
			bufV2.resize(barcnt * sizeof(ZTSBarStruct));
			ZTSBarStruct* newBar = (ZTSBarStruct*)bufV2.data();
			ZTSBarStructOld* oldBar = (ZTSBarStructOld*)buffer.data();
			for (uint32_t idx = 0; idx < barcnt; idx++)
			{
				newBar[idx] = oldBar[idx];
			}
			buffer.swap(bufV2);
		}
		else
		{
			uint32_t tick_cnt = buffer.size() / sizeof(ZTSTickStructOld);
			std::string bufv2;
			bufv2.resize(sizeof(ZTSTickStruct)*tick_cnt);
			ZTSTickStruct* newTick = (ZTSTickStruct*)bufv2.data();
			ZTSTickStructOld* oldTick = (ZTSTickStructOld*)buffer.data();
			for (uint32_t i = 0; i < tick_cnt; i++)
			{
				newTick[i] = oldTick[i];
			}
			buffer.swap(bufv2);
		}
	}

	if (bKeepHead)
	{
		content.resize(BLOCK_HEADER_SIZE);
		content.append(buffer);
		header = (BlockHeader*)content.data();
		header->_version = BLOCK_VERSION_RAW_V2;
	}
	else
	{
		content.swap(buffer);
	}

	return true;
}


uint32_t strToTime(const char* strTime, bool bKeepSec = false)
{
	std::string str;
	const char *pos = strTime;
	while (strlen(pos) > 0)
	{
		if (pos[0] != ':')
		{
			str.append(pos, 1);
		}
		pos++;
	}

	uint32_t ret = strtoul(str.c_str(), NULL, 10);
	if (ret > 10000 && !bKeepSec)
		ret /= 100;

	return ret;
}

uint32_t strToDate(const char* strDate)
{
	StringVector ay = StrUtil::split(strDate, "/");
	if (ay.size() == 1)
		ay = StrUtil::split(strDate, "-");
	std::stringstream ss;
	if (ay.size() > 1)
	{
		auto pos = ay[2].find(" ");
		if (pos != std::string::npos)
			ay[2] = ay[2].substr(0, pos);
		ss << ay[0] << (ay[1].size() == 1 ? "0" : "") << ay[1] << (ay[2].size() == 1 ? "0" : "") << ay[2];
	}
	else
		ss << ay[0];

	return strtoul(ss.str().c_str(), NULL, 10);
}

void dump_bars(ZTString binFolder, ZTString csvFolder, ZTString strFilter /* = "" */, FuncLogCallback cbLogger /* = NULL */)
{
	std::string srcFolder = StrUtil::standardisePath(binFolder);
	if (!BoostFile::exists(srcFolder.c_str()))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("目录%s不存在", binFolder).c_str());
		return;
	}

	if (!BoostFile::exists(csvFolder))
		BoostFile::create_directories(csvFolder);

	boost::filesystem::path myPath(srcFolder);
	boost::filesystem::directory_iterator endIter;
	for (boost::filesystem::directory_iterator iter(myPath); iter != endIter; iter++)
	{
		if (boost::filesystem::is_directory(iter->path()))
			continue;

		if (iter->path().extension() != ".dsb")
			continue;

		const std::string& path = iter->path().string();

		std::string fileCode = iter->path().stem().string();

		if (cbLogger)
			cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

		std::string buffer;
		BoostFile::read_file_contents(path.c_str(), buffer);
		if (buffer.size() < sizeof(HisKlineBlock))
		{
			if (cbLogger)
				cbLogger(StrUtil::printf("文件%s头部校验失败", binFolder).c_str());
			continue;
		}

		BlockHeader* bHeader = (BlockHeader*)buffer.data();

		if(bHeader->_type < BT_HIS_Minute1 || bHeader->_type > BT_HIS_Day)
		{
			if (cbLogger)
				cbLogger(StrUtil::printf("文件%s不是K线数据，跳过转换", binFolder).c_str());
			continue;
		}

		bool isDay = (bHeader->_type == BT_HIS_Day);

		proc_block_data(buffer, true, false);		

		auto kcnt = buffer.size() / sizeof(ZTSBarStruct);
		if (kcnt <= 0)
			continue;

		std::string filename = StrUtil::standardisePath(csvFolder);
		filename += fileCode;
		filename += ".csv";

		if (cbLogger)
			cbLogger(StrUtil::printf("正在写入%s...", filename.c_str()).c_str());

		ZTSBarStruct* bars = (ZTSBarStruct*)buffer.data();

		std::stringstream ss;
		ss << "date,time,open,high,low,close,settle,volume,turnover,open_interest,diff_interest" << std::endl;
		ss.setf(std::ios::fixed);

		for (uint32_t i = 0; i < kcnt; i++)
		{
			const ZTSBarStruct& curBar = bars[i];
			if(isDay)
			{
				ss << curBar.date << ",0,";
			}
			else
			{
				uint32_t barTime = (uint32_t)(curBar.time % 10000 * 100);
				uint32_t barDate = (uint32_t)(curBar.time / 10000 + 19900000);
				ss << barDate << ","
					<< barTime << ",";
			}
			
			ss << curBar.open << ","
				<< curBar.high << ","
				<< curBar.low << ","
				<< curBar.close << ","
				<< curBar.settle << ","
				<< curBar.vol << ","
				<< curBar.money << ","
				<< curBar.hold << ","
				<< curBar.add << std::endl;
		}

		BoostFile::write_file_contents(filename.c_str(), ss.str().c_str(), (uint32_t)ss.str().size());

		if (cbLogger)
			cbLogger(StrUtil::printf("%s写入完成,共%u条bar", filename.c_str(), kcnt).c_str());
	}

	if (cbLogger)
		cbLogger(StrUtil::printf("目录%s全部导出完成...", binFolder).c_str());
}

void dump_ticks(ZTString binFolder, ZTString csvFolder, ZTString strFilter /* = "" */, FuncLogCallback cbLogger /* = NULL */)
{
	std::string srcFolder = StrUtil::standardisePath(binFolder);
	if (!BoostFile::exists(srcFolder.c_str()))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("目录%s不存在", binFolder).c_str());
		return;
	}

	if (!BoostFile::exists(csvFolder))
		BoostFile::create_directories(csvFolder);

	boost::filesystem::path myPath(srcFolder);
	boost::filesystem::directory_iterator endIter;
	for (boost::filesystem::directory_iterator iter(myPath); iter != endIter; iter++)
	{
		if (boost::filesystem::is_directory(iter->path()))
			continue;

		if (iter->path().extension() != ".dsb")
			continue;

		const std::string& path = iter->path().string();

		std::string fileCode = iter->path().stem().string();

		if (cbLogger)
			cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

		std::string buffer;
		BoostFile::read_file_contents(path.c_str(), buffer);
		if (buffer.size() < sizeof(HisTickBlock))
		{
			if (cbLogger)
				cbLogger(StrUtil::printf("文件%s头部校验失败", binFolder).c_str());
			continue;
		}

		proc_block_data(buffer, false, false);

		auto tcnt = buffer.size() / sizeof(ZTSTickStruct);
		if (tcnt <= 0)
			continue;

		std::string filename = StrUtil::standardisePath(csvFolder);
		filename += fileCode;
		filename += ".csv";

		if (cbLogger)
			cbLogger(StrUtil::printf("正在写入%s...", filename.c_str()).c_str());

		ZTSTickStruct* ticks = (ZTSTickStruct*)buffer.data();

		std::stringstream ss;
		ss.setf(std::ios::fixed, std::ios::floatfield);
		ss.precision(6);
		ss << "exchg,code,tradingdate,actiondate,actiontime,price,open,high,low,settle,preclose,"
			<< "presettle,preinterest,total_volume,total_turnover,open_interest,volume,turnover,additional,";
		for (int i = 0; i < 10; i++)
		{
			bool hasTail = (i != 9);
			ss << "bidprice" << i + 1 << "," << "bidqty" << i + 1 << "," << "askprice" << i + 1 << "," << "askqty" << i + 1 << (hasTail ? "," : "");
		}
		ss << std::endl;

		for (uint32_t i = 0; i < tcnt; i++)
		{
			const ZTSTickStruct& curTick = ticks[i];
			ss << curTick.exchg << "," << curTick.code << ","
				<< curTick.trading_date << ","
				<< curTick.action_date << ","
				<< curTick.action_time << ","
				<< curTick.price << ","
				<< curTick.open << ","
				<< curTick.high << ","
				<< curTick.low << ","
				<< curTick.settle_price << ","
				<< curTick.pre_close << ","
				<< curTick.pre_settle << ","
				<< curTick.pre_interest << ","
				<< curTick.total_volume << ","
				<< curTick.total_turnover << ","
				<< curTick.open_interest << ","
				<< curTick.volume << ","
				<< curTick.turn_over << ","
				<< curTick.diff_interest << ",";

			for (int j = 0; j < 10; j++)
			{
				bool hasTail = (j != 9);
				ss << curTick.bid_prices[j] << "," << curTick.bid_qty[j] << "," << curTick.ask_prices[j] << "," << curTick.ask_qty[j] << (hasTail ? "," : "");
			}
			ss << std::endl;
		}

		BoostFile::write_file_contents(filename.c_str(), ss.str().c_str(), (uint32_t)ss.str().size());

		if (cbLogger)
			cbLogger(StrUtil::printf("%s写入完成,共%u条tick数据", filename.c_str(), tcnt).c_str());
	}

	if (cbLogger)
		cbLogger(StrUtil::printf("目录%s全部导出完成...", binFolder).c_str());
}

void trans_csv_bars(ZTString csvFolder, ZTString binFolder, ZTString period, FuncLogCallback cbLogger /* = NULL */)
{
	if (!BoostFile::exists(csvFolder))
		return;

	if (!BoostFile::exists(binFolder))
		BoostFile::create_directories(binFolder);

	ZTSKlinePeriod kp = KP_DAY;
	if (vvt_stricmp(period, "m1") == 0)
		kp = KP_Minute1;
	else if (vvt_stricmp(period, "m5") == 0)
		kp = KP_Minute5;
	else
		kp = KP_DAY;

	boost::filesystem::path myPath(csvFolder);
	boost::filesystem::directory_iterator endIter;
	for (boost::filesystem::directory_iterator iter(myPath); iter != endIter; iter++)
	{
		if (boost::filesystem::is_directory(iter->path()))
			continue;

		if (iter->path().extension() != ".csv")
			continue;

		const std::string& path = iter->path().string();

		if(cbLogger)
			cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

		CsvReader reader(",");
		if(!reader.load_from_file(path.c_str()))
		{
			if (cbLogger)
				cbLogger(StrUtil::printf("读取数据文件%s失败...", path.c_str()).c_str());
			continue;
		}

		std::vector<ZTSBarStruct> bars;

		while(reader.next_row())
		{
			//逐行读取
			ZTSBarStruct bs;
			bs.date = strToDate(reader.get_string("date"));
			if(kp != KP_DAY)
				bs.time = TimeUtils::timeToMinBar(bs.date, strToTime(reader.get_string("time")));
			bs.open = reader.get_double("open");
			bs.high = reader.get_double("high");
			bs.low = reader.get_double("low");
			bs.close = reader.get_double("close");
			bs.vol = reader.get_double("volume");
			bs.money = reader.get_double("turnover");
			bs.hold = reader.get_double("open_interest");
			bs.add = reader.get_double("diff_interest");
			bs.settle = reader.get_double("settle");
			bars.emplace_back(bs);

			if (bars.size() % 1000 == 0)
			{
				if (cbLogger)
					cbLogger(StrUtil::printf("已读取数据%u条", bars.size()).c_str());
			}
		}
		if (cbLogger)
			cbLogger(StrUtil::printf("数据文件%s全部读取完成,共%u条", path.c_str(), bars.size()).c_str());

		BlockType btype;
		switch (kp)
		{
		case KP_Minute1: btype = BT_HIS_Minute1; break;
		case KP_Minute5: btype = BT_HIS_Minute5; break;
		default: btype = BT_HIS_Day; break;
		}

		HisKlineBlockV2 kBlock;
		strcpy(kBlock._blk_flag, BLK_FLAG);
		kBlock._type = btype;
		kBlock._version = BLOCK_VERSION_CMP_V2;

		std::string cmprsData = ZTSCmpHelper::compress_data(bars.data(), sizeof(ZTSBarStruct)*bars.size());
		kBlock._size = cmprsData.size();

		std::string filename = StrUtil::standardisePath(binFolder);
		filename += iter->path().stem().string();
		filename += ".dsb";

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			bf.write_file(&kBlock, sizeof(HisKlineBlockV2));
		}
		bf.write_file(cmprsData);
		bf.close_file();
		if (cbLogger)
			cbLogger(StrUtil::printf("数据已转储至%s", filename.c_str()).c_str());
	}
}

//bool trans_bars(ZTString barFile, FuncGetBarItem getter, int count, ZTString period, FuncLogCallback cbLogger /* = NULL */)
//{
//	if (count == 0)
//	{
//		if (cbLogger)
//			cbLogger("K线数据条数为0");
//		return false;
//	}
//
//	BlockType bType = BT_HIS_Day;
//	if (vvt_stricmp(period, "m1") == 0)
//		bType = BT_HIS_Minute1;
//	else if (vvt_stricmp(period, "m5") == 0)
//		bType = BT_HIS_Minute5;
//	else if(vvt_stricmp(period, "d") == 0)
//		bType = BT_HIS_Day;
//	else
//	{
//		if (cbLogger)
//			cbLogger("周期只能为m1、m5或d");
//		return false;
//	}
//
//	std::string buffer;
//	buffer.resize(sizeof(ZTSBarStruct)*count);
//	ZTSBarStruct* bars = (ZTSBarStruct*)buffer.c_str();
//	int realCnt = 0;
//	for(int i = 0; i < count; i++)
//	{
//		bool bSucc = getter(&bars[i], i);
//		if (!bSucc)
//			break;
//
//		realCnt++;
//	}
//
//	if (realCnt != count)
//	{
//		buffer.resize(sizeof(ZTSBarStruct)*realCnt);
//	}
//
//	if (cbLogger)
//		cbLogger("K线数据已经读取完成，准备写入文件");
//
//	std::string content;
//	content.resize(sizeof(HisKlineBlockV2));
//	HisKlineBlockV2* block = (HisKlineBlockV2*)content.data();
//	strcpy(block->_blk_flag, BLK_FLAG);
//	block->_version = BLOCK_VERSION_CMP;
//	block->_type = bType;
//	std::string cmp_data = ZTSCmpHelper::compress_data(bars, buffer.size());
//	block->_size = cmp_data.size();
//	content.append(cmp_data);
//
//	BoostFile bf;
//	if (bf.create_new_file(barFile))
//	{
//		bf.write_file(content);
//	}
//	bf.close_file();
//
//	if (cbLogger)
//		cbLogger("K线数据写入文件成功");
//	return true;
//}
//
//bool trans_ticks(ZTString tickFile, FuncGetTickItem getter, int count, FuncLogCallback cbLogger/* = NULL*/)
//{
//	if (count == 0)
//	{
//		if (cbLogger)
//			cbLogger("Tick数据条数为0");
//		return false;
//	}
//
//	std::string buffer;
//	buffer.resize(sizeof(ZTSTickStruct)*count);
//	ZTSTickStruct* ticks = (ZTSTickStruct*)buffer.c_str();
//	int realCnt = 0;
//	for (int i = 0; i < count; i++)
//	{
//		bool bSucc = getter(&ticks[i], i);
//		if (!bSucc)
//			break;
//
//		realCnt++;
//	}
//
//	if(realCnt != count)
//	{
//		buffer.resize(sizeof(ZTSTickStruct)*realCnt);
//	}
//
//	if (cbLogger)
//		cbLogger("Tick数据已经读取完成，准备写入文件");
//
//	std::string content;
//	content.resize(sizeof(HisKlineBlockV2));
//	HisKlineBlockV2* block = (HisKlineBlockV2*)content.data();
//	strcpy(block->_blk_flag, BLK_FLAG);
//	block->_version = BLOCK_VERSION_CMP;
//	block->_type = BT_HIS_Ticks;
//	std::string cmp_data = ZTSCmpHelper::compress_data(ticks, buffer.size());
//	block->_size = cmp_data.size();
//	content.append(cmp_data);
//
//	BoostFile bf;
//	if (bf.create_new_file(tickFile))
//	{
//		bf.write_file(content);
//	}
//	bf.close_file();
//
//	if (cbLogger)
//		cbLogger("Tick数据写入文件成功");
//
//	return true;
//}

ZTUInt32 read_dsb_ticks(ZTString tickFile, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger /* = NULL */)
{
	std::string path = tickFile;

	if (cbLogger)
		cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

	std::string content;
	BoostFile::read_file_contents(path.c_str(), content);
	if (content.size() < sizeof(HisTickBlock))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("文件%s头部校验失败", tickFile).c_str());
		return 0;
	}

	proc_block_data(content, false, false);

	if (content.empty())
	{
		cbCnt(0);
		return 0;
	}

	auto tcnt = content.size() / sizeof(ZTSTickStruct);

	cbCnt(tcnt);
	cb((ZTSTickStruct*)content.data(), tcnt, true);

	if (cbLogger)
		cbLogger(StrUtil::printf("%s读取完成,共%u条tick数据", tickFile, tcnt).c_str());

	return (ZTUInt32)tcnt;
}

ZTUInt32 read_dsb_order_details(ZTString dataFile, FuncGetOrdDtlCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger/* = NULL*/)
{
	std::string path = dataFile;

	if (cbLogger)
		cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

	std::string content;
	BoostFile::read_file_contents(path.c_str(), content);
	if (content.size() < sizeof(HisOrdDtlBlock))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("文件%s头部校验失败", dataFile).c_str());
		return 0;
	}

	proc_block_data(content, false, false);

	if (content.empty())
	{
		cbCnt(0);
		return 0;
	}

	auto tcnt = content.size() / sizeof(ZTSOrdDtlStruct);

	cbCnt(tcnt);
	cb((ZTSOrdDtlStruct*)content.data(), tcnt, true);

	if (cbLogger)
		cbLogger(StrUtil::printf("%s读取完成,共%u条order detail数据", dataFile, tcnt).c_str());

	return (ZTUInt32)tcnt;
}

ZTUInt32 read_dsb_order_queues(ZTString dataFile, FuncGetOrdQueCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger/* = NULL*/)
{
	std::string path = dataFile;

	if (cbLogger)
		cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

	std::string content;
	BoostFile::read_file_contents(path.c_str(), content);
	if (content.size() < sizeof(HisOrdQueBlock))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("文件%s头部校验失败", dataFile).c_str());
		return 0;
	}

	proc_block_data(content, false, false);

	if (content.empty())
	{
		cbCnt(0);
		return 0;
	}

	auto tcnt = content.size() / sizeof(ZTSOrdQueStruct);

	cbCnt(tcnt);
	cb((ZTSOrdQueStruct*)content.data(), tcnt, true);

	if (cbLogger)
		cbLogger(StrUtil::printf("%s读取完成,共%u条order queue数据", dataFile, tcnt).c_str());

	return (ZTUInt32)tcnt;
}

ZTUInt32 read_dsb_transactions(ZTString dataFile, FuncGetTransCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger/* = NULL*/)
{
	std::string path = dataFile;

	if (cbLogger)
		cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

	std::string content;
	BoostFile::read_file_contents(path.c_str(), content);
	if (content.size() < sizeof(HisTransBlock))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("文件%s头部校验失败", dataFile).c_str());
		return 0;
	}

	proc_block_data(content, false, false);

	if (content.empty())
	{
		cbCnt(0);
		return 0;
	}

	auto tcnt = content.size() / sizeof(ZTSTransStruct);

	cbCnt(tcnt);
	cb((ZTSTransStruct*)content.data(), tcnt, true);

	if (cbLogger)
		cbLogger(StrUtil::printf("%s读取完成,共%u条transaction数据", dataFile, tcnt).c_str());

	return (ZTUInt32)tcnt;
}

ZTUInt32 read_dsb_bars(ZTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger )
{
	std::string path = barFile;
	if (cbLogger)
		cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

	std::string content;
	BoostFile::read_file_contents(path.c_str(), content);
	if (content.size() < sizeof(HisKlineBlock))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("文件%s头部校验失败", barFile).c_str());
		return 0;
	}

	proc_block_data(content, true, false);

	if(content.empty())
	{
		cbCnt(0);
		return 0;
	}


	auto kcnt = content.size() / sizeof(ZTSBarStruct);
	cbCnt(kcnt);
	cb((ZTSBarStruct*)content.data(), kcnt, true);

	if (cbLogger)
		cbLogger(StrUtil::printf("%s读取完成,共%u条bar", barFile, kcnt).c_str());

	return (ZTUInt32)kcnt;
}

ZTUInt32 read_dmb_bars(ZTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger)
{
	std::string path = barFile;

	std::string buffer;
	BoostFile::read_file_contents(path.c_str(), buffer);
	if (buffer.size() < sizeof(RTKlineBlock))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("文件%s头部校验失败", barFile).c_str());
		return 0;
	}

	RTKlineBlock* tBlock = (RTKlineBlock*)buffer.c_str();
	auto kcnt = tBlock->_size;
	if (kcnt <= 0)
	{
		cbCnt(0);
		return 0;
	}

	cbCnt(kcnt);
	cb(tBlock->_bars, kcnt, true);

	if (cbLogger)
		cbLogger(StrUtil::printf("%s读取完成,共%u条bar", barFile, kcnt).c_str());

	return (ZTUInt32)kcnt;
}

ZTUInt32 read_dmb_ticks(ZTString tickFile, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt, FuncLogCallback cbLogger /* = NULL */)
{
	std::string path = tickFile;

	if (cbLogger)
		cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

	std::string buffer;
	BoostFile::read_file_contents(path.c_str(), buffer);
	if (buffer.size() < sizeof(RTTickBlock))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("文件%s头部校验失败", tickFile).c_str());
		return 0;
	}

	RTTickBlock* tBlock = (RTTickBlock*)buffer.c_str();
	auto tcnt = tBlock->_size;
	if (tcnt <= 0)
	{
		cbCnt(0);
		return 0;
	}

	cbCnt(tcnt);
	cb(tBlock->_ticks, tcnt, true);

	if (cbLogger)
		cbLogger(StrUtil::printf("%s读取完成,共%u条tick数据", tickFile, tcnt).c_str());

	return (ZTUInt32)tcnt;
}


ZTUInt32 resample_bars(ZTString barFile, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt, ZTUInt64 fromTime, ZTUInt64 endTime,
	ZTString period, ZTUInt32 times, ZTString sessInfo, FuncLogCallback cbLogger /* = NULL */, bool bAlignSec/* = false*/)
{
	ZTSKlinePeriod kp;
	if(vvt_stricmp(period, "m1") == 0)
	{
		kp = KP_Minute1;
	}
	else if (vvt_stricmp(period, "m5") == 0)
	{
		kp = KP_Minute5;
	}
	else if (vvt_stricmp(period, "d") == 0)
	{
		kp = KP_DAY;
	}
	else
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("周期%s不是基础周期...", period).c_str());
		return 0;
	}

	bool isDay = (kp == KP_DAY);

	if(isDay)
	{
		if(fromTime >= 100000000 || endTime > 100000000)
		{
			if (cbLogger)
				cbLogger("日线基础数据的开始时间结束时间应为日期，格式如yyyymmdd");
			return 0;
		}
	}
	else
	{
		if (fromTime < 100000000 || endTime < 100000000)
		{
			if (cbLogger)
				cbLogger("分钟线基础数据的开始时间结束时间应为时间，格式如yyyymmddHHMM");
			return 0;
		}
	}

	if(fromTime > endTime)
	{
		std::swap(fromTime, endTime);
	}

	ZTSSessionInfo* sInfo = NULL;
	{
		rj::Document root;
		if (root.Parse(sessInfo).HasParseError())
		{
			if (cbLogger)
				cbLogger("交易时间模板解析失败");
			return 0;
		}

		int32_t offset = root["offset"].GetInt();

		sInfo = ZTSSessionInfo::create("tmp", "tmp", offset);

		if (!root["auction"].IsNull())
		{
			const rj::Value& jAuc = root["auction"];
			sInfo->setAuctionTime(jAuc["from"].GetUint(), jAuc["to"].GetUint());
		}

		const rj::Value& jSecs = root["sections"];
		if (jSecs.IsNull() || !jSecs.IsArray())
		{
			if (cbLogger)
				cbLogger("交易时间模板格式错误");
			return 0;
		}

		for (const rj::Value& jSec : jSecs.GetArray())
		{
			sInfo->addTradingSection(jSec["from"].GetUint(), jSec["to"].GetUint());
		}
	}

	std::string path = barFile;
	if (cbLogger)
		cbLogger(StrUtil::printf("正在读取数据文件%s...", path.c_str()).c_str());

	std::string buffer;
	BoostFile::read_file_contents(path.c_str(), buffer);
	if (buffer.size() < sizeof(HisKlineBlock))
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("文件%s头部校验失败", barFile).c_str());
		return 0;
	}

	proc_block_data(buffer, true, false);

	auto kcnt = buffer.size() / sizeof(ZTSBarStruct);
	if (kcnt <= 0)
	{
		if (cbLogger)
			cbLogger(StrUtil::printf("%s数据为空", barFile).c_str());
		return 0;
	}

	ZTSBarStruct* bars = (ZTSBarStruct*)buffer.c_str();

	//确定第一条K线的位置
	ZTSBarStruct bar;
	if (isDay)
		bar.date = (uint32_t)fromTime;
	else
	{

		bar.time = fromTime % 100000000 + ((fromTime / 100000000) - 1990) * 100000000;
	}

	ZTSBarStruct* pBar = std::lower_bound(bars, bars + (kcnt - 1), bar, [isDay](const ZTSBarStruct& a, const ZTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});


	uint32_t sIdx = (uint32_t)(pBar - bars);
	if((isDay && pBar->date < bar.date) || (!isDay && pBar->time < bar.time))
	{
		//如果返回的K线的时间小于要查找的时间，说明没有符合条件的数据
		if (cbLogger)
			cbLogger("没有找到指定时间范围的K线");
		return 0;
	}
	else if (sIdx != 0 && ((isDay && pBar->date > bar.date) || (!isDay && pBar->time > bar.time)))
	{
		pBar--;
		sIdx--;
	}

	//确定最后一条K线的位置
	if (isDay)
		bar.date = (uint32_t)endTime;
	else
	{

		bar.time = endTime % 100000000 + ((endTime / 100000000) - 1990) * 100000000;
	}
	pBar = std::lower_bound(bars, bars + (kcnt - 1), bar, [isDay](const ZTSBarStruct& a, const ZTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});

	uint32_t eIdx = 0;
	if (pBar == NULL)
		eIdx = kcnt - 1;
	else
		eIdx = (uint32_t)(pBar - bars);

	if (eIdx != 0 && ((isDay && pBar->date > bar.date) || (!isDay && pBar->time > bar.time)))
	{
		pBar--;
		eIdx--;
	}

	uint32_t hitCnt = eIdx - sIdx + 1;
	ZTSKlineSlice* slice = ZTSKlineSlice::create("", kp, 1, &bars[sIdx], hitCnt);
	ZTSDataFactory fact;
	ZTSKlineData* kline = fact.extractKlineData(slice, kp, times, sInfo, true, bAlignSec);
	if(kline == NULL)
	{
		if (cbLogger)
			cbLogger("K线重采样失败");
		return 0;
	}

	uint32_t newCnt = kline->size();
	cbCnt(newCnt);
	cb(&kline->getDataRef().at(0),newCnt, true);

	if (cbLogger)
		cbLogger(StrUtil::printf("%s重采样完成,共将%u条bar重采样为%u条新bar", barFile, hitCnt, newCnt).c_str());

	
	kline->release();
	sInfo->release();
	slice->release();

	return (ZTUInt32)newCnt;
}

bool store_bars(ZTString barFile, ZTSBarStruct* firstBar, int count, ZTString period, FuncLogCallback cbLogger /* = NULL */)
{
	if (count == 0)
	{
		if (cbLogger)
			cbLogger("K线数据条数为0");
		return false;
	}

	BlockType bType = BT_HIS_Day;
	if (vvt_stricmp(period, "m1") == 0)
		bType = BT_HIS_Minute1;
	else if (vvt_stricmp(period, "m5") == 0)
		bType = BT_HIS_Minute5;
	else if (vvt_stricmp(period, "d") == 0)
		bType = BT_HIS_Day;
	else
	{
		if (cbLogger)
			cbLogger("周期只能为m1、m5或d");
		return false;
	}

	std::string buffer;
	buffer.resize(sizeof(ZTSBarStruct)*count);
	ZTSBarStruct* bars = (ZTSBarStruct*)buffer.c_str();
	memcpy(bars, firstBar, sizeof(ZTSBarStruct)*count);

	if (cbLogger)
		cbLogger("K线数据已经读取完成，准备写入文件");

	std::string content;
	content.resize(sizeof(HisKlineBlockV2));
	HisKlineBlockV2* block = (HisKlineBlockV2*)content.data();
	strcpy(block->_blk_flag, BLK_FLAG);
	block->_version = BLOCK_VERSION_CMP_V2;
	block->_type = bType;
	std::string cmp_data = ZTSCmpHelper::compress_data(bars, buffer.size());
	block->_size = cmp_data.size();
	content.append(cmp_data);

	BoostFile bf;
	if (bf.create_new_file(barFile))
	{
		bf.write_file(content);
	}
	bf.close_file();

	if (cbLogger)
		cbLogger("K线数据写入文件成功");
	return true;
}

bool store_ticks(ZTString tickFile, ZTSTickStruct* firstTick, int count, FuncLogCallback cbLogger/* = NULL*/)
{
	if (count == 0)
	{
		if (cbLogger)
			cbLogger("Tick数据条数为0");
		return false;
	}

	std::string buffer;
	buffer.resize(sizeof(ZTSTickStruct)*count);
	ZTSTickStruct* ticks = (ZTSTickStruct*)buffer.c_str();
	memcpy(ticks, firstTick, sizeof(ZTSTickStruct)*count);

	if (cbLogger)
		cbLogger("Tick数据已经读取完成，准备写入文件");

	std::string content;
	content.resize(sizeof(HisTickBlockV2));
	HisTickBlockV2* block = (HisTickBlockV2*)content.data();
	strcpy(block->_blk_flag, BLK_FLAG);
	block->_version = BLOCK_VERSION_CMP_V2;
	block->_type = BT_HIS_Ticks;
	std::string cmp_data = ZTSCmpHelper::compress_data(ticks, buffer.size());
	block->_size = cmp_data.size();
	content.append(cmp_data);

	BoostFile bf;
	if (bf.create_new_file(tickFile))
	{
		bf.write_file(content);
	}
	bf.close_file();

	if (cbLogger)
		cbLogger("Tick数据写入文件成功");

	return true;
}

bool store_order_details(ZTString tickFile, ZTSOrdDtlStruct* firstItem, int count, FuncLogCallback cbLogger/* = NULL*/)
{
	if (count == 0)
	{
		if (cbLogger)
			cbLogger("Size of OrderDetail is 0");
		return false;
	}

	std::string buffer;
	buffer.resize(sizeof(ZTSOrdDtlStruct)*count);
	ZTSOrdDtlStruct* items = (ZTSOrdDtlStruct*)buffer.c_str();
	memcpy(items, firstItem, sizeof(ZTSOrdDtlStruct)*count);

	if (cbLogger)
		cbLogger("Reading order details done, prepare to write...");

	std::string content;
	content.resize(sizeof(HisOrdDtlBlockV2));
	HisOrdDtlBlockV2* block = (HisOrdDtlBlockV2*)content.data();
	strcpy(block->_blk_flag, BLK_FLAG);
	block->_version = BLOCK_VERSION_CMP_V2;
	block->_type = BT_HIS_OrdDetail;
	std::string cmp_data = ZTSCmpHelper::compress_data(items, buffer.size());
	block->_size = cmp_data.size();
	content.append(cmp_data);

	BoostFile bf;
	if (bf.create_new_file(tickFile))
	{
		bf.write_file(content);
	}
	bf.close_file();

	if (cbLogger)
		cbLogger("Writing order details succeed");

	return true;
}

bool store_order_queues(ZTString tickFile, ZTSOrdQueStruct* firstItem, int count, FuncLogCallback cbLogger/* = NULL*/)
{
	if (count == 0)
	{
		if (cbLogger)
			cbLogger("Size of order queues is 0");
		return false;
	}

	std::string buffer;
	buffer.resize(sizeof(ZTSOrdQueStruct)*count);
	ZTSOrdQueStruct* items = (ZTSOrdQueStruct*)buffer.c_str();
	memcpy(items, firstItem, sizeof(ZTSOrdQueStruct)*count);

	if (cbLogger)
		cbLogger("Reading order queues done, prepare to write...");

	std::string content;
	content.resize(sizeof(HisOrdQueBlockV2));
	HisOrdQueBlockV2* block = (HisOrdQueBlockV2*)content.data();
	strcpy(block->_blk_flag, BLK_FLAG);
	block->_version = BLOCK_VERSION_CMP_V2;
	block->_type = BT_HIS_OrdQueue;
	std::string cmp_data = ZTSCmpHelper::compress_data(items, buffer.size());
	block->_size = cmp_data.size();
	content.append(cmp_data);

	BoostFile bf;
	if (bf.create_new_file(tickFile))
	{
		bf.write_file(content);
	}
	bf.close_file();

	if (cbLogger)
		cbLogger("Writing order queues to file succeedd");

	return true;
}

bool store_transactions(ZTString tickFile, ZTSTransStruct* firstItem, int count, FuncLogCallback cbLogger/* = NULL*/)
{
	if (count == 0)
	{
		if (cbLogger)
			cbLogger("Size of transations is 0");
		return false;
	}

	std::string buffer;
	buffer.resize(sizeof(ZTSTransStruct)*count);
	ZTSTransStruct* items = (ZTSTransStruct*)buffer.c_str();
	memcpy(items, firstItem, sizeof(ZTSTransStruct)*count);

	if (cbLogger)
		cbLogger("Reading transactions done, prepare to write...");

	std::string content;
	content.resize(sizeof(HisTransBlockV2));
	HisTransBlockV2* block = (HisTransBlockV2*)content.data();
	strcpy(block->_blk_flag, BLK_FLAG);
	block->_version = BLOCK_VERSION_CMP_V2;
	block->_type = BT_HIS_Trnsctn;
	std::string cmp_data = ZTSCmpHelper::compress_data(items, buffer.size());
	block->_size = cmp_data.size();
	content.append(cmp_data);

	BoostFile bf;
	if (bf.create_new_file(tickFile))
	{
		bf.write_file(content);
	}
	bf.close_file();

	if (cbLogger)
		cbLogger("Write transactions to file succeedd");

	return true;
}