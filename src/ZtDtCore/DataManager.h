/*!
 * \file DataManager.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据管理器定义
 */
#pragma once

#include "../Includes/IDataWriter.h"
#include "../Share/StdUtils.hpp"
#include "../Share/BoostMappingFile.hpp"

NS_ZTP_BEGIN
class ZTSTickData;
class ZTSOrdQueData;
class ZTSOrdDtlData;
class ZTSTransData;
class ZTSVariant;
class IDataCaster;
NS_ZTP_END

USING_NS_ZTP;

class ZTSBaseDataMgr;
class StateMonitor;
class UDPCaster;

class DataManager : public IDataWriterSink
{
public:
	DataManager();
	~DataManager();

public:
	bool init(ZTSVariant* params, ZTSBaseDataMgr* bdMgr, StateMonitor* stMonitor);

	void add_ext_dumper(const char* id, IHisDataDumper* dumper);

	inline void add_caster(IDataCaster* caster)
	{
		if (caster == NULL)
			return;

		_casters.emplace_back(caster);
	}

	void release();

	bool writeTick(ZTSTickData* curTick, uint32_t procFlag);

	bool writeOrderQueue(ZTSOrdQueData* curOrdQue);

	bool writeOrderDetail(ZTSOrdDtlData* curOrdDetail);

	bool writeTransaction(ZTSTransData* curTrans);

	void transHisData(const char* sid);
	
	bool isSessionProceeded(const char* sid);

	ZTSTickData* getCurTick(const char* code, const char* exchg = "");

public:
	//////////////////////////////////////////////////////////////////////////
	//IDataWriterSink
	virtual IBaseDataMgr* getBDMgr() override;

	virtual bool canSessionReceive(const char* sid) override;

	virtual void broadcastTick(ZTSTickData* curTick) override;

	virtual void broadcastOrdQue(ZTSOrdQueData* curOrdQue) override;

	virtual void broadcastOrdDtl(ZTSOrdDtlData* curOrdDtl) override;

	virtual void broadcastTrans(ZTSTransData* curTrans) override;

	virtual CodeSet* getSessionComms(const char* sid) override;

	virtual uint32_t getTradingDate(const char* pid) override;

	/*
	*	处理解析模块的日志
	*	@ll			日志级别
	*	@message	日志内容
	*/
	virtual void outputLog(ZTSLogLevel ll, const char* message) override;

private:
	IDataWriter*		_writer;
	FuncDeleteWriter	_remover;
	ZTSBaseDataMgr*		_bd_mgr;
	StateMonitor*		_state_mon;
	std::vector<IDataCaster*>	_casters;
};

