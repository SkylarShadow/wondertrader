#pragma once
#include <functional>
#include "ZtLocalExecuter.h"

NS_ZTP_BEGIN
class ZtFilterMgr;

typedef std::function<void(ExecCmdPtr)> EnumExecuterCb;

class ZtExecuterMgr : private boost::noncopyable
{
public:
	ZtExecuterMgr():_filter_mgr(NULL){}

	inline void set_filter_mgr(ZtFilterMgr* mgr) { _filter_mgr = mgr; }

	inline void	add_executer(ExecCmdPtr executer)
	{
		_executers[executer->name()] = executer;
	}

	void	enum_executer(EnumExecuterCb cb);

	void	set_positions(vvt_hashmap<std::string, double> target_pos);
	void	handle_pos_change(const char* stdCode, double targetPos, double diffPos, const char* execid = "ALL");
	void	handle_tick(const char* stdCode, ZTSTickData* curTick);

	/*
	 *	加载路由规则
	 */
	bool	load_router_rules(ZTSVariant* config);

	/*
	 *	
	 */
	inline const vvt_hashset<std::string>& get_route(const char* strategyid)
	{
		static vvt_hashset<std::string> ALL_EXECUTERS;
		if (ALL_EXECUTERS.empty())
			ALL_EXECUTERS.insert("ALL");

		if (_router_rules.empty())
			return ALL_EXECUTERS;

		auto it = _router_rules.find(strategyid);
		if (it == _router_rules.end())
			return ALL_EXECUTERS;

		return it->second;
	}

	/*
	 *	清除缓存的目标仓位
	 */
	inline void	clear_cached_targets()
	{
		_all_cached_targets.clear();
	}

	/*
	 *	将目标仓位加入缓存
	 *	@stdCode	合约代码
	 *	@targetPos	目标仓位
	 *	@execid		执行器ID
	 */
	void	add_target_to_cache(const char* stdCode, double targetPos, const char* execid = "ALL");

	/*
	 *	提交缓存的目标头寸
	 *	@scale	风控系数
	 */
	void	commit_cached_targets(double scale = 1.0);

private:
	typedef vvt_hashmap<std::string, ExecCmdPtr> ExecuterMap;
	ExecuterMap		_executers;
	ZtFilterMgr*	_filter_mgr;

	typedef vvt_hashmap<std::string, double> TargetsMap;
	vvt_hashmap<std::string, TargetsMap>	_all_cached_targets;

	typedef vvt_hashset<std::string>	ExecuterSet;
	vvt_hashmap<std::string, ExecuterSet>	_router_rules;

	vvt_hashset<std::string>	_routed_executers;
};
NS_ZTP_END
