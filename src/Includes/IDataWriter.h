/*!
 * \file IDataWriter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据落地接口定义
 */
#pragma once
#include <stdint.h>
#include "ZTSTypes.h"
#include "FasterDefs.h"

NS_ZTP_BEGIN
class ZTSTickData;
class ZTSOrdQueData;
class ZTSOrdDtlData;
class ZTSTransData;
class ZTSVariant;
class IBaseDataMgr;
struct ZTSBarStruct;
struct ZTSTickStruct;
struct ZTSOrdDtlStruct;
struct ZTSOrdQueStruct;
struct ZTSTransStruct;

class IDataWriterSink
{
public:
	
	virtual IBaseDataMgr* getBDMgr() = 0;

	virtual bool canSessionReceive(const char* sid) = 0;

	virtual void broadcastTick(ZTSTickData* curTick) = 0;

	virtual void broadcastOrdQue(ZTSOrdQueData* curOrdQue) = 0;

	virtual void broadcastOrdDtl(ZTSOrdDtlData* curOrdDtl) = 0;

	virtual void broadcastTrans(ZTSTransData* curTrans) = 0;

	virtual CodeSet* getSessionComms(const char* sid) = 0;

	virtual uint32_t getTradingDate(const char* pid) = 0;

	/*
	*	处理解析模块的日志
	*	@ll			日志级别
	*	@message	日志内容
	*/
	virtual void outputLog(ZTSLogLevel ll, const char* message) = 0;
};

class IHisDataDumper
{
public:
	virtual bool dumpHisBars(const char* stdCode, const char* period, ZTSBarStruct* bars, uint32_t count) = 0;
	virtual bool dumpHisTicks(const char* stdCode, uint32_t uDate, ZTSTickStruct* ticks, uint32_t count) = 0;

	virtual bool dumpHisOrdQue(const char* stdCode, uint32_t uDate, ZTSOrdQueStruct* items, uint32_t count) { return false; }
	virtual bool dumpHisOrdDtl(const char* stdCode, uint32_t uDate, ZTSOrdDtlStruct* items, uint32_t count) { return false; }
	virtual bool dumpHisTrans(const char* stdCode, uint32_t uDate, ZTSTransStruct* items, uint32_t count) { return false; }
};

typedef vvt_hashmap<std::string, IHisDataDumper*> ExtDumpers;

/*
 *	数据落地接口
 */
class IDataWriter
{
public:
	IDataWriter():_sink(NULL){}

	virtual bool init(ZTSVariant* params, IDataWriterSink* sink) { _sink = sink; return true; }

	virtual void release() = 0;

	void	add_ext_dumper(const char* id, IHisDataDumper* dumper) { _dumpers[id] = dumper; }

public:
	virtual bool writeTick(ZTSTickData* curTick, uint32_t procFlag) = 0;

	virtual bool writeOrderQueue(ZTSOrdQueData* curOrdQue) { return false; }

	virtual bool writeOrderDetail(ZTSOrdDtlData* curOrdDetail) { return false; }

	virtual bool writeTransaction(ZTSTransData* curTrans) { return false; }

	virtual void transHisData(const char* sid) {}

	virtual bool isSessionProceeded(const char* sid) { return true; }

	virtual ZTSTickData* getCurTick(const char* code, const char* exchg = "") = 0;

protected:
	ExtDumpers			_dumpers;
	IDataWriterSink*	_sink;
};

NS_ZTP_END


//获取IDataWriter的函数指针类型
typedef vvtp::IDataWriter* (*FuncCreateWriter)();
typedef void(*FuncDeleteWriter)(vvtp::IDataWriter* &writer);