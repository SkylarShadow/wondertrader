/*!
 * \file WTSBaseDataMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 基础数据管理器实现
 */
#pragma once
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/VvTSCollection.hpp"
#include "../Includes/FasterDefs.h"

USING_NS_VVTP;

typedef vvt_hashmap<std::string, TradingDayTpl>	TradingDayTplMap;

typedef VvTSHashMap<std::string>		WTSContractList;
typedef VvTSHashMap<std::string>		WTSExchgContract;
typedef VvTSHashMap<std::string>		WTSContractMap;

typedef VvTSHashMap<std::string>		WTSSessionMap;
typedef VvTSHashMap<std::string>		WTSCommodityMap;

typedef vvt_hashmap<std::string, CodeSet> SessionCodeMap;

class WTSBaseDataMgr : public IBaseDataMgr
{
public:
	WTSBaseDataMgr();
	~WTSBaseDataMgr();

public:
	virtual VvTSCommodityInfo*	getCommodity(const char* stdPID) override;
	virtual VvTSCommodityInfo*	getCommodity(const char* exchg, const char* pid) override;

	virtual VvTSContractInfo*	getContract(const char* code, const char* exchg = "", uint32_t uDate = 0) override;
	virtual VvTSArray*			getContracts(const char* exchg = "", uint32_t uDate = 0) override;

	virtual VvTSSessionInfo*		getSession(const char* sid) override;
	virtual VvTSSessionInfo*		getSessionByCode(const char* code, const char* exchg = "") override;
	virtual VvTSArray*			getAllSessions() override;
	virtual bool				isHoliday(const char* stdPID, uint32_t uDate, bool isTpl = false) override;

	virtual uint32_t			calcTradingDate(const char* stdPID, uint32_t uDate, uint32_t uTime, bool isSession = false) override;
	virtual uint64_t			getBoundaryTime(const char* stdPID, uint32_t tDate, bool isSession = false, bool isStart = true) override;

	virtual uint32_t			getContractSize(const char* exchg = "", uint32_t uDate = 0) override;

	void		release();

	bool		loadSessions(const char* filename);
	bool		loadCommodities(const char* filename);
	bool		loadContracts(const char* filename);
	bool		loadHolidays(const char* filename);

public:
	uint32_t	getTradingDate(const char* stdPID, uint32_t uOffDate = 0, uint32_t uOffMinute = 0, bool isTpl = false);
	uint32_t	getNextTDate(const char* stdPID, uint32_t uDate, int days = 1, bool isTpl = false);
	uint32_t	getPrevTDate(const char* stdPID, uint32_t uDate, int days = 1, bool isTpl = false);
	bool		isTradingDate(const char* stdPID, uint32_t uDate, bool isTpl = false);
	void		setTradingDate(const char* stdPID, uint32_t uDate, bool isTpl = false);

	CodeSet*	getSessionComms(const char* sid);

private:
	const char* getTplIDByPID(const char* stdPID);

private:
	TradingDayTplMap	m_mapTradingDay;

	SessionCodeMap		m_mapSessionCode;

	WTSExchgContract*	m_mapExchgContract;
	WTSSessionMap*		m_mapSessions;
	WTSCommodityMap*	m_mapCommodities;
	WTSContractMap*		m_mapContracts;
};

