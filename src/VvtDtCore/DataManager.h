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

NS_VVTP_BEGIN
class VvTSTickData;
class VvTSOrdQueData;
class VvTSOrdDtlData;
class VvTSTransData;
class VvTSVariant;
class IDataCaster;
NS_VVTP_END

USING_NS_VVTP;

class VvTSBaseDataMgr;
class StateMonitor;
class UDPCaster;

class DataManager : public IDataWriterSink
{
public:
	DataManager();
	~DataManager();

public:
	bool init(VvTSVariant* params, VvTSBaseDataMgr* bdMgr, StateMonitor* stMonitor);

	void add_ext_dumper(const char* id, IHisDataDumper* dumper);

	inline void add_caster(IDataCaster* caster)
	{
		if (caster == NULL)
			return;

		_casters.emplace_back(caster);
	}

	void release();

	bool writeTick(VvTSTickData* curTick, uint32_t procFlag);

	bool writeOrderQueue(VvTSOrdQueData* curOrdQue);

	bool writeOrderDetail(VvTSOrdDtlData* curOrdDetail);

	bool writeTransaction(VvTSTransData* curTrans);

	void transHisData(const char* sid);
	
	bool isSessionProceeded(const char* sid);

	VvTSTickData* getCurTick(const char* code, const char* exchg = "");

public:
	//////////////////////////////////////////////////////////////////////////
	//IDataWriterSink
	virtual IBaseDataMgr* getBDMgr() override;

	virtual bool canSessionReceive(const char* sid) override;

	virtual void broadcastTick(VvTSTickData* curTick) override;

	virtual void broadcastOrdQue(VvTSOrdQueData* curOrdQue) override;

	virtual void broadcastOrdDtl(VvTSOrdDtlData* curOrdDtl) override;

	virtual void broadcastTrans(VvTSTransData* curTrans) override;

	virtual CodeSet* getSessionComms(const char* sid) override;

	virtual uint32_t getTradingDate(const char* pid) override;

	/*
	*	处理解析模块的日志
	*	@ll			日志级别
	*	@message	日志内容
	*/
	virtual void outputLog(VvTSLogLevel ll, const char* message) override;

private:
	IDataWriter*		_writer;
	FuncDeleteWriter	_remover;
	VvTSBaseDataMgr*		_bd_mgr;
	StateMonitor*		_state_mon;
	std::vector<IDataCaster*>	_casters;
};

