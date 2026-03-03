#pragma once

#include "WtSimpDataMgr.h"

#include "../WtCore/WtExecMgr.h"
#include "../WtCore/TraderAdapter.h"
#include "../WtCore/ParserAdapter.h"
#include "../WtCore/ActionPolicyMgr.h"

#include "../VvTSTools/VvTSHotMgr.h"
#include "../VvTSTools/VvTSBaseDataMgr.h"

NS_VVTP_BEGIN
class VvTSVariant;
NS_VVTP_END

USING_NS_VVTP;

class WtExecRunner : public IParserStub, public IExecuterStub
{
public:
	WtExecRunner();

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

	VvTSSessionInfo* get_session_info(const char* sid, bool isCode = true);

	//////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 处理实时主推行情
	/// </summary>
	/// <param name="curTick">最新的tick数据</param>
	/// <param name="isHot">是否是主力合约代码</param>
	virtual void handle_push_quote(VvTSTickData* curTick) override;

	///////////////////////////////////////////////////////////////////////////
	//IExecuterStub 接口
	virtual uint64_t get_real_time() override;
	virtual VvTSCommodityInfo* get_comm_info(const char* stdCode) override;
	virtual VvTSSessionInfo* get_sess_info(const char* stdCode) override;
	virtual IHotMgr* get_hot_mon() override { return &_hot_mgr; }
	virtual uint32_t get_trading_day() override;

private:
	bool initTraders(VvTSVariant* cfgTrader);
	bool initParsers(VvTSVariant* cfgParser);
	bool initExecuters(VvTSVariant* cfgExecuter);
	bool initDataMgr();
	bool initActionPolicy();

private:
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;
	WtExecuterFactory	_exe_factory;
	WtExecuterMgr		_exe_mgr;

	VvTSVariant*			_config;

	WtSimpDataMgr		_data_mgr;

	VvTSBaseDataMgr		_bd_mgr;
	VvTSHotMgr			_hot_mgr;
	ActionPolicyMgr		_act_policy;

	vvt_hashmap<std::string, double> _positions;
};

