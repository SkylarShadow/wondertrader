/*!
 * \file ZTSBaseDataMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ZTSBaseDataMgr.h"
#include "../ZTSUtils/ZTSCfgLoader.h"
#include "ZTSLogger.h"

#include "../Includes/ZTSContractInfo.hpp"
#include "../Includes/ZTSSessionInfo.hpp"
#include "../Includes/ZTSVariant.hpp"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

const char* DEFAULT_HOLIDAY_TPL = "CHINA";

ZTSBaseDataMgr::ZTSBaseDataMgr()
	: m_mapExchgContract(NULL)
	, m_mapSessions(NULL)
	, m_mapCommodities(NULL)
	, m_mapContracts(NULL)
{
	m_mapExchgContract = ZTSExchgContract::create();
	m_mapSessions = ZTSSessionMap::create();
	m_mapCommodities = ZTSCommodityMap::create();
	m_mapContracts = ZTSContractMap::create();
}


ZTSBaseDataMgr::~ZTSBaseDataMgr()
{
	if (m_mapExchgContract)
	{
		m_mapExchgContract->release();
		m_mapExchgContract = NULL;
	}

	if (m_mapSessions)
	{
		m_mapSessions->release();
		m_mapSessions = NULL;
	}

	if (m_mapCommodities)
	{
		m_mapCommodities->release();
		m_mapCommodities = NULL;
	}

	if(m_mapContracts)
	{
		m_mapContracts->release();
		m_mapContracts = NULL;
	}
}

ZTSCommodityInfo* ZTSBaseDataMgr::getCommodity(const char* exchgpid)
{
	return (ZTSCommodityInfo*)m_mapCommodities->get(exchgpid);
}


ZTSCommodityInfo* ZTSBaseDataMgr::getCommodity(const char* exchg, const char* pid)
{
	if (m_mapCommodities == NULL)
		return NULL;

	char key[64] = { 0 };
	fmt::format_to(key, "{}.{}", exchg, pid);

	return (ZTSCommodityInfo*)m_mapCommodities->get(key);
}


ZTSContractInfo* ZTSBaseDataMgr::getContract(const char* code, const char* exchg /* = "" */, uint32_t uDate /* = 0 */)
{
	//如果直接找到对应的市场代码,则直接
	
	auto lKey = std::string(code);

	if (strlen(exchg) == 0)
	{
		auto it = m_mapContracts->find(lKey);
		if (it == m_mapContracts->end())
			return NULL;

		ZTSArray* ayInst = (ZTSArray*)it->second;
		if (ayInst == NULL || ayInst->size() == 0)
			return NULL;

		for(std::size_t i = 0; i < ayInst->size(); i++)
		{
			ZTSContractInfo* cInfo = (ZTSContractInfo*)ayInst->at(i);
			/*
			 *	By Wesley @ 2023.10.23
			 *	if param uDate is not zero, need to check whether contract is valid
			 */
			if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
				return cInfo;
		}
		return NULL;
	}
	else
	{
		auto sKey = std::string(exchg);
		auto it = m_mapExchgContract->find(sKey);
		if (it != m_mapExchgContract->end())
		{
			ZTSContractList* contractList = (ZTSContractList*)it->second;
			auto it = contractList->find(lKey);
			if (it != contractList->end())
			{
				ZTSContractInfo* cInfo = (ZTSContractInfo*)it->second;
				/*
				 *	By Wesley @ 2023.10.23
				 *	if param uDate is not zero, need to check whether contract is valid
				 */
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					return cInfo;
			}

			return NULL;
		}
	}

	return NULL;
}

uint32_t  ZTSBaseDataMgr::getContractSize(const char* exchg /* = "" */, uint32_t uDate /* = 0 */)
{
	uint32_t ret = 0;
	if (strlen(exchg) > 0)
	{
		auto it = m_mapExchgContract->find(std::string(exchg));
		if (it != m_mapExchgContract->end())
		{
			ZTSContractList* contractList = (ZTSContractList*)it->second;
			auto it2 = contractList->begin();
			for (; it2 != contractList->end(); it2++)
			{
				ZTSContractInfo* cInfo = (ZTSContractInfo*)it2->second;
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					ret++;
			}
		}
	}
	else
	{
		auto it = m_mapExchgContract->begin();
		for (; it != m_mapExchgContract->end(); it++)
		{
			ZTSContractList* contractList = (ZTSContractList*)it->second;
			auto it2 = contractList->begin();
			for (; it2 != contractList->end(); it2++)
			{
				ZTSContractInfo* cInfo = (ZTSContractInfo*)it2->second;
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					ret++;
			}
		}
	}

	return ret;
}

ZTSArray* ZTSBaseDataMgr::getContracts(const char* exchg /* = "" */, uint32_t uDate /* = 0 */)
{
	ZTSArray* ay = ZTSArray::create();
	if(strlen(exchg) > 0)
	{
		auto it = m_mapExchgContract->find(std::string(exchg));
		if (it != m_mapExchgContract->end())
		{
			ZTSContractList* contractList = (ZTSContractList*)it->second;
			auto it2 = contractList->begin();
			for (; it2 != contractList->end(); it2++)
			{
				ZTSContractInfo* cInfo = (ZTSContractInfo*)it2->second;
				/*
				 *	By Wesley @ 2023.10.23
				 *	if param uDate is not zero, need to check whether contract is valid
				 */
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					ay->append(cInfo, true);
			}
		}
	}
	else
	{
		auto it = m_mapExchgContract->begin();
		for (; it != m_mapExchgContract->end(); it++)
		{
			ZTSContractList* contractList = (ZTSContractList*)it->second;
			auto it2 = contractList->begin();
			for (; it2 != contractList->end(); it2++)
			{
				ZTSContractInfo* cInfo = (ZTSContractInfo*)it2->second;
				/*
				 *	By Wesley @ 2023.10.23
				 *	if param uDate is not zero, need to check whether contract is valid
				 */
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					ay->append(cInfo, true);
			}
		}
	}

	return ay;
}

ZTSArray* ZTSBaseDataMgr::getAllSessions()
{
	ZTSArray* ay = ZTSArray::create();
	for (auto it = m_mapSessions->begin(); it != m_mapSessions->end(); it++)
	{
		ay->append(it->second, true);
	}
	return ay;
}

ZTSSessionInfo* ZTSBaseDataMgr::getSession(const char* sid)
{
	return (ZTSSessionInfo*)m_mapSessions->get(sid);
}

ZTSSessionInfo* ZTSBaseDataMgr::getSessionByCode(const char* code, const char* exchg /* = "" */)
{
	ZTSContractInfo* ct = getContract(code, exchg);
	if (ct == NULL)
		return NULL;

	return ct->getCommInfo()->getSessionInfo();
}

bool ZTSBaseDataMgr::isHoliday(const char* pid, uint32_t uDate, bool isTpl /* = false */)
{
	uint32_t wd = TimeUtils::getWeekDay(uDate);
	if (wd == 0 || wd == 6)
		return true;

	std::string tplid = pid;
	if (!isTpl)
		tplid = getTplIDByPID(pid);

	auto it = m_mapTradingDay.find(tplid.c_str());
	if(it != m_mapTradingDay.end())
	{
		const TradingDayTpl& tpl = it->second;
		return (tpl._holidays.find(uDate) != tpl._holidays.end());
	}

	return false;
}


void ZTSBaseDataMgr::release()
{
	if (m_mapExchgContract)
	{
		m_mapExchgContract->release();
		m_mapExchgContract = NULL;
	}

	if (m_mapSessions)
	{
		m_mapSessions->release();
		m_mapSessions = NULL;
	}

	if (m_mapCommodities)
	{
		m_mapCommodities->release();
		m_mapCommodities = NULL;
	}
}

bool ZTSBaseDataMgr::loadSessions(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		ZTSLogger::error("Trading sessions configuration file {} not exists", filename);
		return false;
	}

	ZTSVariant* root = ZTSCfgLoader::load_from_file(filename);
	if (root == NULL)
	{
		ZTSLogger::error("Loading session config file {} failed", filename);
		return false;
	}

	for(const std::string& id : root->memberNames())
	{
		ZTSVariant* jVal = root->get(id);

		const char* name = jVal->getCString("name");
		int32_t offset = jVal->getInt32("offset");

		ZTSSessionInfo* sInfo = ZTSSessionInfo::create(id.c_str(), name, offset);

		if (jVal->has("auction"))
		{
			ZTSVariant* jAuc = jVal->get("auction");
			sInfo->setAuctionTime(jAuc->getUInt32("from"), jAuc->getUInt32("to"));
		}
		else if (jVal->has("auctions"))
		{
			ZTSVariant* jAucs = jVal->get("auctions");
			for (uint32_t i = 0; i < jAucs->size(); i++)
			{
				ZTSVariant* jSec = jAucs->get(i);
				sInfo->addAuctionTime(jSec->getUInt32("from"), jSec->getUInt32("to"));
			}
		}

		ZTSVariant* jSecs = jVal->get("sections");
		if (jSecs == NULL || !jSecs->isArray())
			continue;

		for (uint32_t i = 0; i < jSecs->size(); i++)
		{
			ZTSVariant* jSec = jSecs->get(i);
			sInfo->addTradingSection(jSec->getUInt32("from"), jSec->getUInt32("to"));
		}

		m_mapSessions->add(id.c_str(), sInfo);
	}

	root->release();

	return true;
}

void parseCommodity(ZTSCommodityInfo* pCommInfo, ZTSVariant* jPInfo)
{
	pCommInfo->setPriceTick(jPInfo->getDouble("pricetick"));
	pCommInfo->setVolScale(jPInfo->getUInt32("volscale"));

	if (jPInfo->has("category"))
		pCommInfo->setCategory((ContractCategory)jPInfo->getUInt32("category"));
	else
		pCommInfo->setCategory(CC_Future);

	pCommInfo->setCoverMode((CoverMode)jPInfo->getUInt32("covermode"));
	pCommInfo->setPriceMode((PriceMode)jPInfo->getUInt32("pricemode"));

	if (jPInfo->has("trademode"))
		pCommInfo->setTradingMode((TradingMode)jPInfo->getUInt32("trademode"));
	else
		pCommInfo->setTradingMode(TM_Both);

	double lotsTick = 1;
	double minLots = 1;
	if (jPInfo->has("lotstick"))
		lotsTick = jPInfo->getDouble("lotstick");
	if (jPInfo->has("minlots"))
		minLots = jPInfo->getDouble("minlots");
	pCommInfo->setLotsTick(lotsTick);
	pCommInfo->setMinLots(minLots);
}

bool ZTSBaseDataMgr::loadCommodities(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		ZTSLogger::error("Commodities configuration file {} not exists", filename);
		return false;
	}

	ZTSVariant* root = ZTSCfgLoader::load_from_file(filename);
	if (root == NULL)
	{
		ZTSLogger::error("Loading commodities config file {} failed", filename);
		return false;
	}

	for(const std::string& exchg : root->memberNames())
	{
		ZTSVariant* jExchg = root->get(exchg);

		for (const std::string& pid : jExchg->memberNames())
		{
			ZTSVariant* jPInfo = jExchg->get(pid);

			const char* name = jPInfo->getCString("name");
			const char* sid = jPInfo->getCString("session");
			const char* hid = jPInfo->getCString("holiday");

			if (strlen(sid) == 0)
			{
				ZTSLogger::warn("No session configured for {}.{}", exchg.c_str(), pid.c_str());
				continue;
			}

			ZTSCommodityInfo* pCommInfo = ZTSCommodityInfo::create(pid.c_str(), name, exchg.c_str(), sid, hid);
			parseCommodity(pCommInfo, jPInfo);

			ZTSSessionInfo* sInfo = getSession(sid);
			pCommInfo->setSessionInfo(sInfo);

			std::string key = fmt::format("{}.{}", exchg.c_str(), pid.c_str());
			if (m_mapCommodities == NULL)
				m_mapCommodities = ZTSCommodityMap::create();

			m_mapCommodities->add(key, pCommInfo, false);

			m_mapSessionCode[sid].insert(key);
		}
	}

	ZTSLogger::info("Commodities configuration file {} loaded", filename);
	root->release();
	return true;
}

bool ZTSBaseDataMgr::loadContracts(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		ZTSLogger::error("Contracts configuration file {} not exists", filename);
		return false;
	}

	ZTSVariant* root = ZTSCfgLoader::load_from_file(filename);
	if (root == NULL)
	{
		ZTSLogger::error("Loading contracts config file {} failed", filename);
		return false;
	}

	for(const std::string& exchg : root->memberNames())
	{
		ZTSVariant* jExchg = root->get(exchg);

		for(const std::string& code : jExchg->memberNames())
		{
			ZTSVariant* jcInfo = jExchg->get(code);

			/*
			 *	By Wesley @ 2021.12.28
			 *	这里做一个兼容，如果product为空,先检查是否配置了rules属性，如果配置了rules属性，把合约单独当成品种自动加入
			 *	如果没有配置rules，则直接跳过该合约
			 */
			ZTSCommodityInfo* commInfo = NULL;
			std::string pid;
			if(jcInfo->has("product"))
			{
				pid = jcInfo->getCString("product");
				commInfo = getCommodity(jcInfo->getCString("exchg"), pid.c_str());
			}
			else if(jcInfo->has("rules"))
			{
				pid = code.c_str();
				ZTSVariant* jPInfo = jcInfo->get("rules");
				const char* name = jcInfo->getCString("name");
				std::string sid = jPInfo->getCString("session");
				std::string hid;
				if(jPInfo->has("holiday"))
					hid = jPInfo->getCString("holiday");

				//这里不能像解析commodity那样处理，直接赋值为ALLDAY
				if (sid.empty())
					sid = "ALLDAY";

				commInfo = ZTSCommodityInfo::create(pid.c_str(), name, exchg.c_str(), sid.c_str(), hid.c_str());
				parseCommodity(commInfo, jPInfo);
				ZTSSessionInfo* sInfo = getSession(sid.c_str());
				commInfo->setSessionInfo(sInfo);

				std::string key = fmt::format("{}.{}", exchg.c_str(), pid.c_str());
				if (m_mapCommodities == NULL)
					m_mapCommodities = ZTSCommodityMap::create();

				m_mapCommodities->add(key, commInfo, false);

				m_mapSessionCode[sid].insert(key);

				ZTSLogger::debug("Commodity {} has been automatically added", key.c_str());
			}

			if (commInfo == NULL)
			{
				ZTSLogger::warn("Commodity {}.{} not found, contract {} skipped", jcInfo->getCString("exchg"), jcInfo->getCString("product"), code.c_str());
				continue;
			}

			ZTSContractInfo* cInfo = ZTSContractInfo::create(code.c_str(),
				jcInfo->getCString("name"),
				jcInfo->getCString("exchg"),
				pid.c_str());

			cInfo->setCommInfo(commInfo);

			uint32_t maxMktQty = 1000000;
			uint32_t maxLmtQty = 1000000;
			uint32_t minMktQty = 1;
			uint32_t minLmtQty = 1;
			if (jcInfo->has("maxmarketqty"))
				maxMktQty = jcInfo->getUInt32("maxmarketqty");
			if (jcInfo->has("maxlimitqty"))
				maxLmtQty = jcInfo->getUInt32("maxlimitqty");
			if (jcInfo->has("minmarketqty"))
				minMktQty = jcInfo->getUInt32("minmarketqty");
			if (jcInfo->has("minlimitqty"))
				minLmtQty = jcInfo->getUInt32("minlimitqty");
			cInfo->setVolumeLimits(maxMktQty, maxLmtQty, minMktQty, minLmtQty);

			uint32_t opendate = 0;
			uint32_t expiredate = 0;
			if (jcInfo->has("opendate"))
				opendate = jcInfo->getUInt32("opendate");
			if (jcInfo->has("expiredate"))
				expiredate = jcInfo->getUInt32("expiredate");
			cInfo->setDates(opendate, expiredate);

			double lMargin = 0;
			double sMargin = 0;
			if (jcInfo->has("longmarginratio"))
				lMargin = jcInfo->getDouble("longmarginratio");
			if (jcInfo->has("shortmarginratio"))
				sMargin = jcInfo->getDouble("shortmarginratio");
			cInfo->setMarginRatios(lMargin, sMargin);

			ZTSContractList* contractList = (ZTSContractList*)m_mapExchgContract->get(std::string(cInfo->getExchg()));
			if (contractList == NULL)
			{
				contractList = ZTSContractList::create();
				m_mapExchgContract->add(std::string(cInfo->getExchg()), contractList, false);
			}
			contractList->add(std::string(cInfo->getCode()), cInfo, false);

			commInfo->addCode(code.c_str());

			std::string key = std::string(cInfo->getCode());
			ZTSArray* ayInst = (ZTSArray*)m_mapContracts->get(key);
			if(ayInst == NULL)
			{
				ayInst = ZTSArray::create();
				m_mapContracts->add(key, ayInst, false);
			}

			ayInst->append(cInfo, true);
		}
	}

	ZTSLogger::info("Contracts configuration file {} loaded, {} exchanges", filename, m_mapExchgContract->size());
	root->release();
	return true;
}

bool ZTSBaseDataMgr::loadHolidays(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		ZTSLogger::error("Holidays configuration file {} not exists", filename);
		return false;
	}

	ZTSVariant* root = ZTSCfgLoader::load_from_file(filename);
	if (root == NULL)
	{
		ZTSLogger::error("Loading holidays config file {} failed", filename);
		return false;
	}

	for (const std::string& hid : root->memberNames())
	{
		ZTSVariant* jHolidays = root->get(hid);
		if(!jHolidays->isArray())
			continue;

		TradingDayTpl& trdDayTpl = m_mapTradingDay[hid];
		for(uint32_t i = 0; i < jHolidays->size(); i++)
		{
			ZTSVariant* hItem = jHolidays->get(i);
			trdDayTpl._holidays.insert(hItem->asUInt32());
		}
	}

	root->release();

	return true;
}

uint64_t ZTSBaseDataMgr::getBoundaryTime(const char* stdPID, uint32_t tDate, bool isSession /* = false */, bool isStart /* = true */)
{
	if(tDate == 0)
		tDate = TimeUtils::getCurDate();
	
	std::string tplid = stdPID;
	bool isTpl = false;
	ZTSSessionInfo* sInfo = NULL;
	if (isSession)
	{
		sInfo = getSession(stdPID);
		tplid = DEFAULT_HOLIDAY_TPL;
		isTpl = true;
	}
	else
	{
		ZTSCommodityInfo* cInfo = getCommodity(stdPID);
		if (cInfo == NULL)
			return 0;

		sInfo = cInfo->getSessionInfo();
	}

	if (sInfo == NULL)
		return 0;

	uint32_t weekday = TimeUtils::getWeekDay(tDate);
	if (weekday == 6 || weekday == 0)
	{
		if (isStart)
			tDate = getNextTDate(tplid.c_str(), tDate, 1, isTpl);
		else
			tDate = getPrevTDate(tplid.c_str(), tDate, 1, isTpl);
	}

	//不偏移的最简单,只需要直接返回开盘和收盘时间即可
	if (sInfo->getOffsetMins() == 0)
	{
		if (isStart)
			return (uint64_t)tDate * 10000 + sInfo->getOpenTime();
		else
			return (uint64_t)tDate * 10000 + sInfo->getCloseTime();
	}

	if(sInfo->getOffsetMins() < 0)
	{
		//往前偏移,就是交易日推后,一般用于外盘
		//这个比较简单,只需要按自然日取即可
		if (isStart)
			return (uint64_t)tDate * 10000 + sInfo->getOpenTime();
		else
			return (uint64_t)TimeUtils::getNextDate(tDate) * 10000 + sInfo->getCloseTime();
	}
	else
	{
		//往后偏移,一般国内期货夜盘都是这个,即夜盘算是第二个交易日
		//这个比较复杂,主要节假日后第一天的边界很麻烦（如一般情况的周一）
		//这种情况唯一方便的就是,收盘时间不需要处理
		if(!isStart)
			return (uint64_t)tDate * 10000 + sInfo->getCloseTime();

		//想到一个简单的办法,就是不管怎么样,开始时间一定是上一个交易日的晚上
		//所以我只需要拿到上一个交易日即可
		tDate = getPrevTDate(tplid.c_str(), tDate, 1, isTpl);
		return (uint64_t)tDate * 10000 + sInfo->getOpenTime();
	}
}

uint32_t ZTSBaseDataMgr::calcTradingDate(const char* stdPID, uint32_t uDate, uint32_t uTime, bool isSession /* = false */)
{
	if (uDate == 0)
	{
		TimeUtils::getDateTime(uDate, uTime);
		uTime /= 100000;
	}

	std::string tplid = stdPID;
	bool isTpl = false;
	ZTSSessionInfo* sInfo = NULL;
	if(isSession)
	{
		sInfo = getSession(stdPID);
		tplid = DEFAULT_HOLIDAY_TPL;
		isTpl = true;
	}
	else
	{
		ZTSCommodityInfo* cInfo = getCommodity(stdPID);
		if (cInfo == NULL)
			return uDate;
		
		sInfo = cInfo->getSessionInfo();
	}

	if (sInfo == NULL)
		return uDate;
	
	uint32_t offMin = sInfo->offsetTime(uTime, true);
	//7*24交易时间单独算
	uint32_t total_mins = sInfo->getTradingMins();
	if(total_mins == 1440)
	{
		if(sInfo->getOffsetMins() > 0 && uTime > offMin)
		{
			return TimeUtils::getNextDate(uDate, 1);
		}
		else if (sInfo->getOffsetMins() < 0 && uTime < offMin)
		{
			return TimeUtils::getNextDate(uDate, -1);
		}

		return uDate;
	}

	uint32_t weekday = TimeUtils::getWeekDay(uDate);
	if (sInfo->getOffsetMins() > 0)
	{
		//如果向后偏移,且当前时间大于偏移时间,说明向后跨日了
		//这时交易日=下一个交易日
		if (uTime > offMin)
		{
			//如,20151016 23:00,偏移300分钟,为5:00
			return getNextTDate(tplid.c_str(), uDate, 1, isTpl);
		}
		else if (weekday == 6 || weekday == 0)
		{
			//如,20151017 1:00,周六,交易日为20151019
			return getNextTDate(tplid.c_str(), uDate, 1, isTpl);
		}
	}
	else if (sInfo->getOffsetMins() < 0)
	{
		//如果向前偏移,且当前时间小于偏移时间,说明还是前一个交易日
		//这时交易日=前一个交易日
		if (uTime < offMin)
		{
			//如20151017 1:00,偏移-300分钟,为20:00
			return getPrevTDate(tplid.c_str(), uDate, 1, isTpl);
		}
		else if (weekday == 6 || weekday == 0)
		{
			//因为向前偏移,如果在周末,则直接到下一个交易日
			return getNextTDate(tplid.c_str(), uDate, 1, isTpl);
		}
	}
	else if (weekday == 6 || weekday == 0)
	{
		//如果没有偏移,且在周末,则直接读取下一个交易日
		return getNextTDate(tplid.c_str(), uDate, 1, isTpl);;
	}

	//其他情况,交易日=自然日
	return uDate;
}

uint32_t ZTSBaseDataMgr::getTradingDate(const char* pid, uint32_t uOffDate /* = 0 */, uint32_t uOffMinute /* = 0 */, bool isTpl /* = false */)
{
	const char* tplID = isTpl ? pid : getTplIDByPID(pid);

	uint32_t curDate = TimeUtils::getCurDate();
	auto it = m_mapTradingDay.find(tplID);
	if (it == m_mapTradingDay.end())
	{
		return curDate;
	}

	TradingDayTpl* tpl = (TradingDayTpl*)&it->second;
	if (tpl->_cur_tdate != 0 && uOffDate == 0)
		return tpl->_cur_tdate;

	if (uOffDate == 0)
		uOffDate = curDate;

	uint32_t weekday = TimeUtils::getWeekDay(uOffDate);

	if (weekday == 6 || weekday == 0)
	{
		//如果没有偏移,且在周末,则直接读取下一个交易日
		tpl->_cur_tdate = getNextTDate(tplID, uOffDate, 1, true);
		uOffDate = tpl->_cur_tdate;
	}

	//其他情况,交易日=自然日
	return uOffDate;
}

uint32_t ZTSBaseDataMgr::getNextTDate(const char* pid, uint32_t uDate, int days /* = 1 */, bool isTpl /* = false */)
{
	uint32_t curDate = uDate;
	int left = days;
	while (true)
	{
		tm t;
		memset(&t, 0, sizeof(tm));
		t.tm_year = curDate / 10000 - 1900;
		t.tm_mon = (curDate % 10000) / 100 - 1;
		t.tm_mday = curDate % 100;
		//t.tm_isdst 	
		time_t ts = mktime(&t);
		ts += 86400;

		tm* newT = localtime(&ts);
		curDate = (newT->tm_year + 1900) * 10000 + (newT->tm_mon + 1) * 100 + newT->tm_mday;
		if (newT->tm_wday != 0 && newT->tm_wday != 6 && !isHoliday(pid, curDate, isTpl))
		{
			//如果不是周末,也不是节假日,则剩余的天数-1
			left--;
			if (left == 0)
				return curDate;
		}
	}
}

uint32_t ZTSBaseDataMgr::getPrevTDate(const char* pid, uint32_t uDate, int days /* = 1 */, bool isTpl /* = false */)
{
	uint32_t curDate = uDate;
	int left = days;
	while (true)
	{
		tm t;
		memset(&t, 0, sizeof(tm));
		t.tm_year = curDate / 10000 - 1900;
		t.tm_mon = (curDate % 10000) / 100 - 1;
		t.tm_mday = curDate % 100;
		//t.tm_isdst 	
		time_t ts = mktime(&t);
		ts -= 86400;

		tm* newT = localtime(&ts);
		curDate = (newT->tm_year + 1900) * 10000 + (newT->tm_mon + 1) * 100 + newT->tm_mday;
		if (newT->tm_wday != 0 && newT->tm_wday != 6 && !isHoliday(pid, curDate, isTpl))
		{
			//如果不是周末,也不是节假日,则剩余的天数-1
			left--;
			if (left == 0)
				return curDate;
		}
	}
}

bool ZTSBaseDataMgr::isTradingDate(const char* pid, uint32_t uDate, bool isTpl /* = false */)
{
	uint32_t wd = TimeUtils::getWeekDay(uDate);
	if (wd != 0 && wd != 6 && !isHoliday(pid, uDate, isTpl))
	{
		return true;
	}

	return false;
}

void ZTSBaseDataMgr::setTradingDate(const char* pid, uint32_t uDate, bool isTpl /* = false */)
{
	std::string tplID = pid;
	if (!isTpl)
		tplID = getTplIDByPID(pid);

	auto it = m_mapTradingDay.find(tplID);
	if (it == m_mapTradingDay.end())
		return;

	TradingDayTpl* tpl = (TradingDayTpl*)&it->second;
	tpl->_cur_tdate = uDate;
}


CodeSet* ZTSBaseDataMgr::getSessionComms(const char* sid)
{
	auto it = m_mapSessionCode.find(sid);
	if (it == m_mapSessionCode.end())
		return NULL;

	return (CodeSet*)&it->second;
}

const char* ZTSBaseDataMgr::getTplIDByPID(const char* pid)
{
	const StringVector& ay = StrUtil::split(pid, ".");
	ZTSCommodityInfo* commInfo = getCommodity(ay[0].c_str(), ay[1].c_str());
	if (commInfo == NULL)
		return "";

	return commInfo->getTradingTpl();
}