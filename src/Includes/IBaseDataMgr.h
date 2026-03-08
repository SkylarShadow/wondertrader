/*!
 * \file IBaseDataMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 基础数据管理器接口定义
 */
#pragma once
#include <string>
#include <stdint.h>

#include "ZTSMarcos.h"
#include "FasterDefs.h"

NS_ZTP_BEGIN
typedef CodeSet ContractSet;

class ZTSContractInfo;
class ZTSArray;
class ZTSSessionInfo;
class ZTSCommodityInfo;

typedef vvt_hashset<uint32_t> HolidaySet;
typedef struct _TradingDayTpl
{
	uint32_t	_cur_tdate;
	HolidaySet	_holidays;

	_TradingDayTpl() :_cur_tdate(0){}
} TradingDayTpl;

class IBaseDataMgr
{
public:
	virtual ZTSCommodityInfo*	getCommodity(const char* exchgpid)						= 0;
	virtual ZTSCommodityInfo*	getCommodity(const char* exchg, const char* pid)		= 0;

	virtual ZTSContractInfo*	getContract(const char* code, const char* exchg = "", uint32_t uDate = 0)	= 0;
	virtual ZTSArray*			getContracts(const char* exchg = "", uint32_t uDate = 0)					= 0;

	virtual ZTSSessionInfo*		getSession(const char* sid)						= 0;
	virtual ZTSSessionInfo*		getSessionByCode(const char* code, const char* exchg = "") = 0;
	virtual ZTSArray*			getAllSessions() = 0;

	virtual bool				isHoliday(const char* pid, uint32_t uDate, bool isTpl = false) = 0;

	virtual uint32_t			calcTradingDate(const char* stdPID, uint32_t uDate, uint32_t uTime, bool isSession = false) = 0;
	virtual uint64_t			getBoundaryTime(const char* stdPID, uint32_t tDate, bool isSession = false, bool isStart = true) = 0;

	virtual uint32_t			getContractSize(const char* exchg = "", uint32_t uDate = 0) { return 0; }
};
NS_ZTP_END