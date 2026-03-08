/*!
 * \file ZtDtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "PorterDefs.h"
#include "ExpDumper.h"

#include "../ZtDtCore/DataManager.h"
#include "../ZtDtCore/ParserAdapter.h"
#include "../ZtDtCore/StateMonitor.h"
#include "../ZtDtCore/UDPCaster.h"
#include "../ZtDtCore/IndexFactory.h"
#include "../ZtDtCore/ShmCaster.h"

#include "../ZTSTools/ZTSHotMgr.h"
#include "../ZTSTools/ZTSBaseDataMgr.h"

#include <boost/asio.hpp>

NS_ZTP_BEGIN
class ZTSVariant;
NS_ZTP_END

class ZtDtRunner
{
public:
	ZtDtRunner();
	~ZtDtRunner();

public:
	void	initialize(const char* cfgFile, const char* logCfg, const char* modDir = "", bool bCfgFile = true, bool bLogCfgFile = true);
	void	start(bool bAsync = false, bool bAlldayMode = false);

	bool	createExtParser(const char* id);


//////////////////////////////////////////////////////////////////////////
//扩展Parser
public:
	void registerParserPorter(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub);

	void parser_init(const char* id);
	void parser_connect(const char* id);
	void parser_release(const char* id);
	void parser_disconnect(const char* id);
	void parser_subscribe(const char* id, const char* code);
	void parser_unsubscribe(const char* id, const char* code);

	void on_ext_parser_quote(const char* id, ZTSTickStruct* curTick, uint32_t uProcFlag);

//////////////////////////////////////////////////////////////////////////
//扩展Dumper
public:
	bool createExtDumper(const char* id);

	void registerExtDumper(FuncDumpBars barDumper, FuncDumpTicks tickDumper);

	void registerExtHftDataDumper(FuncDumpOrdQue ordQueDumper, FuncDumpOrdDtl ordDtlDumper, FuncDumpTrans transDumper);

	bool dumpHisBars(const char* id, const char* stdCode, const char* period, ZTSBarStruct* bars, uint32_t count);

	bool dumpHisTicks(const char* id, const char* stdCode, uint32_t uDate, ZTSTickStruct* ticks, uint32_t count);

	bool dumpHisOrdQue(const char* id, const char* stdCode, uint32_t uDate, ZTSOrdQueStruct* item, uint32_t count);

	bool dumpHisOrdDtl(const char* id, const char* stdCode, uint32_t uDate, ZTSOrdDtlStruct* items, uint32_t count);

	bool dumpHisTrans(const char* id, const char* stdCode, uint32_t uDate, ZTSTransStruct* items, uint32_t count);

private:
	void initDataMgr(ZTSVariant* config, bool bAlldayMode = false);
	void initParsers(ZTSVariant* cfg);

private:

	ZTSBaseDataMgr	_bd_mgr;
	ZTSHotMgr		_hot_mgr;
	boost::asio::io_service _async_io;
	StateMonitor	_state_mon;
	UDPCaster		_udp_caster;
	ShmCaster		_shm_caster;
	DataManager		_data_mgr;
	IndexFactory	_idx_factory;
	ParserAdapterMgr	_parsers;

	FuncParserEvtCallback	_cb_parser_evt;
	FuncParserSubCallback	_cb_parser_sub;

	FuncDumpBars	_dumper_for_bars;
	FuncDumpTicks	_dumper_for_ticks;

	FuncDumpOrdQue	_dumper_for_ordque;
	FuncDumpOrdDtl	_dumper_for_orddtl;
	FuncDumpTrans	_dumper_for_trans;

	typedef std::shared_ptr<ExpDumper> ExpDumperPtr;
	typedef std::map<std::string, ExpDumperPtr>  ExpDumpers;
	ExpDumpers		_dumpers;

	bool _to_exit;
};

