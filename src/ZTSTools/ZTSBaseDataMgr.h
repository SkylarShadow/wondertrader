/*!
 * \file ZTSBaseDataMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 基础数据管理器实现
 */
#pragma once
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/ZTSCollection.hpp"
#include "../Includes/FasterDefs.h"

USING_NS_ZTP;

typedef vvt_hashmap<std::string, TradingDayTpl>	TradingDayTplMap;

typedef ZTSHashMap<std::string>		ZTSContractList;
typedef ZTSHashMap<std::string>		ZTSExchgContract;
typedef ZTSHashMap<std::string>		ZTSContractMap;

typedef ZTSHashMap<std::string>		ZTSSessionMap;
typedef ZTSHashMap<std::string>		ZTSCommodityMap;

typedef vvt_hashmap<std::string, CodeSet> SessionCodeMap;

class ZTSBaseDataMgr : public IBaseDataMgr
{
public:
	ZTSBaseDataMgr();
	~ZTSBaseDataMgr();

public:
	virtual ZTSCommodityInfo*	getCommodity(const char* stdPID) override;
	virtual ZTSCommodityInfo*	getCommodity(const char* exchg, const char* pid) override;

	virtual ZTSContractInfo*	getContract(const char* code, const char* exchg = "", uint32_t uDate = 0) override;
	virtual ZTSArray*			getContracts(const char* exchg = "", uint32_t uDate = 0) override;

	virtual ZTSSessionInfo*		getSession(const char* sid) override;
	virtual ZTSSessionInfo*		getSessionByCode(const char* code, const char* exchg = "") override;
	virtual ZTSArray*			getAllSessions() override;
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

	ZTSExchgContract*	m_mapExchgContract;
	ZTSSessionMap*		m_mapSessions;
	ZTSCommodityMap*	m_mapCommodities;
	ZTSContractMap*		m_mapContracts;
};

