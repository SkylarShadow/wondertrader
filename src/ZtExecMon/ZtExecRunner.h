#pragma once

#include "ZtSimpDataMgr.h"

#include "../ZtCore/ZtExecMgr.h"
#include "../ZtCore/TraderAdapter.h"
#include "../ZtCore/ParserAdapter.h"
#include "../ZtCore/ActionPolicyMgr.h"

#include "../ZTSTools/ZTSHotMgr.h"
#include "../ZTSTools/ZTSBaseDataMgr.h"

NS_ZTP_BEGIN
class ZTSVariant;
NS_ZTP_END

USING_NS_ZTP;

class ZtExecRunner : public IParserStub, public IExecuterStub
{
public:
	ZtExecRunner();

	/*
	 *	初始化
	 */
	bool init(const char* logCfg = "logcfgexec.json", bool isFile = true);

	bool config(const char* cfgFile, bool isFile = true);

	void run();

	void release();

	void setPosition(const char* stdCode, double targetPos);

	void commitPositions();

	bool addExeFactories(const char* folder);

	IBaseDataMgr*	get_bd_mgr() { return &_bd_mgr; }

	IHotMgr* get_hot_mgr() { return &_hot_mgr; }

	ZTSSessionInfo* get_session_info(const char* sid, bool isCode = true);

	//////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 处理实时主推行情
	/// </summary>
	/// <param name="curTick">最新的tick数据</param>
	/// <param name="isHot">是否是主力合约代码</param>
	virtual void handle_push_quote(ZTSTickData* curTick) override;

	///////////////////////////////////////////////////////////////////////////
	//IExecuterStub 接口
	virtual uint64_t get_real_time() override;
	virtual ZTSCommodityInfo* get_comm_info(const char* stdCode) override;
	virtual ZTSSessionInfo* get_sess_info(const char* stdCode) override;
	virtual IHotMgr* get_hot_mon() override { return &_hot_mgr; }
	virtual uint32_t get_trading_day() override;

private:
	bool initTraders(ZTSVariant* cfgTrader);
	bool initParsers(ZTSVariant* cfgParser);
	bool initExecuters(ZTSVariant* cfgExecuter);
	bool initDataMgr();
	bool initActionPolicy();

private:
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;
	ZtExecuterFactory	_exe_factory;
	ZtExecuterMgr		_exe_mgr;

	ZTSVariant*			_config;

	ZtSimpDataMgr		_data_mgr;

	ZTSBaseDataMgr		_bd_mgr;
	ZTSHotMgr			_hot_mgr;
	ActionPolicyMgr		_act_policy;

	vvt_hashmap<std::string, double> _positions;
};

