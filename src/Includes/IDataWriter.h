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
#include "VvTSTypes.h"
#include "FasterDefs.h"

NS_VVTP_BEGIN
class VvTSTickData;
class VvTSOrdQueData;
class VvTSOrdDtlData;
class VvTSTransData;
class VvTSVariant;
class IBaseDataMgr;
struct VvTSBarStruct;
struct VvTSTickStruct;
struct VvTSOrdDtlStruct;
struct VvTSOrdQueStruct;
struct VvTSTransStruct;

class IDataWriterSink
{
public:
	
	virtual IBaseDataMgr* getBDMgr() = 0;

	virtual bool canSessionReceive(const char* sid) = 0;

	virtual void broadcastTick(VvTSTickData* curTick) = 0;

	virtual void broadcastOrdQue(VvTSOrdQueData* curOrdQue) = 0;

	virtual void broadcastOrdDtl(VvTSOrdDtlData* curOrdDtl) = 0;

	virtual void broadcastTrans(VvTSTransData* curTrans) = 0;

	virtual CodeSet* getSessionComms(const char* sid) = 0;

	virtual uint32_t getTradingDate(const char* pid) = 0;

	/*
	*	处理解析模块的日志
	*	@ll			日志级别
	*	@message	日志内容
	*/
	virtual void outputLog(VvTSLogLevel ll, const char* message) = 0;
};

class IHisDataDumper
{
public:
	virtual bool dumpHisBars(const char* stdCode, const char* period, VvTSBarStruct* bars, uint32_t count) = 0;
	virtual bool dumpHisTicks(const char* stdCode, uint32_t uDate, VvTSTickStruct* ticks, uint32_t count) = 0;

	virtual bool dumpHisOrdQue(const char* stdCode, uint32_t uDate, VvTSOrdQueStruct* items, uint32_t count) { return false; }
	virtual bool dumpHisOrdDtl(const char* stdCode, uint32_t uDate, VvTSOrdDtlStruct* items, uint32_t count) { return false; }
	virtual bool dumpHisTrans(const char* stdCode, uint32_t uDate, VvTSTransStruct* items, uint32_t count) { return false; }
};

typedef vvt_hashmap<std::string, IHisDataDumper*> ExtDumpers;

/*
 *	数据落地接口
 */
class IDataWriter
{
public:
	IDataWriter():_sink(NULL){}

	virtual bool init(VvTSVariant* params, IDataWriterSink* sink) { _sink = sink; return true; }

	virtual void release() = 0;

	void	add_ext_dumper(const char* id, IHisDataDumper* dumper) { _dumpers[id] = dumper; }

public:
	virtual bool writeTick(VvTSTickData* curTick, uint32_t procFlag) = 0;

	virtual bool writeOrderQueue(VvTSOrdQueData* curOrdQue) { return false; }

	virtual bool writeOrderDetail(VvTSOrdDtlData* curOrdDetail) { return false; }

	virtual bool writeTransaction(VvTSTransData* curTrans) { return false; }

	virtual void transHisData(const char* sid) {}

	virtual bool isSessionProceeded(const char* sid) { return true; }

	virtual VvTSTickData* getCurTick(const char* code, const char* exchg = "") = 0;

protected:
	ExtDumpers			_dumpers;
	IDataWriterSink*	_sink;
};

NS_VVTP_END


//获取IDataWriter的函数指针类型
typedef vvtp::IDataWriter* (*FuncCreateWriter)();
typedef void(*FuncDeleteWriter)(vvtp::IDataWriter* &writer);