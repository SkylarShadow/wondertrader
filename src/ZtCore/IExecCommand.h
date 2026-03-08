#pragma once
#include "../Includes/FasterDefs.h"
#include <stdint.h>

NS_ZTP_BEGIN
class ZTSCommodityInfo;
class ZTSSessionInfo;
class IHotMgr;
class ZTSTickData;

class IExecuterStub
{
public:
	virtual uint64_t get_real_time() = 0;
	virtual ZTSCommodityInfo* get_comm_info(const char* stdCode) = 0;
	virtual ZTSSessionInfo* get_sess_info(const char* stdCode) = 0;
	virtual IHotMgr* get_hot_mon() = 0;
	virtual uint32_t get_trading_day() = 0;
};

class IExecCommand
{
public:
	IExecCommand(const char* name) :_stub(NULL), _name(name){}
	/*
	 *	设置目标仓位
	 */
	virtual void set_position(const vvt_hashmap<std::string, double>& targets) {}

	/*
	 *	合约仓位变动
	 */
	virtual void on_position_changed(const char* stdCode, double diffPos) {}

	/*
	 *	实时行情回调
	 */
	virtual void on_tick(const char* stdCode, ZTSTickData* newTick) {}


	inline void setStub(IExecuterStub* stub) { _stub = stub; }

	inline const char* name() const { return _name.c_str(); }

	inline void setName(const char* name) { _name = name; }

protected:
	IExecuterStub*	_stub;
	std::string		_name;
};
NS_ZTP_END