/*!
 * \file ZTSHotMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ZTSHotMgr.h"
#include "../ZTSUtils/ZTSCfgLoader.h"

#include "../Includes/ZTSSwitchItem.hpp"
#include "../Includes/ZTSVariant.hpp"

#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/decimal.h"


ZTSHotMgr::ZTSHotMgr()
	: m_mapCustRules(NULL)
	, m_bInitialized(false)
{
}


ZTSHotMgr::~ZTSHotMgr()
{
}

const char* ZTSHotMgr::getRuleTag(const char* stdCode)
{
	if (m_mapCustRules == NULL)
		return "";

	auto len = strlen(stdCode);
	if (stdCode[len - 1] == '+' || stdCode[len - 1] == '-')
		len--;

	auto idx = StrUtil::findLast(stdCode, '.');
	if (idx == std::string::npos)
	{
		auto it = m_mapCustRules->find(std::string(stdCode, len));
		if (it == m_mapCustRules->end())
			return "";

		return it->first.c_str();
	}

	const char* tail = stdCode + idx + 1;
	auto it = m_mapCustRules->find(std::string(tail, len - idx - 1));
	if (it == m_mapCustRules->end())
		return "";

	return it->first.c_str();
}

double ZTSHotMgr::getRuleFactor(const char* ruleTag, const char* fullPid, uint32_t uDate /* = 0 */ )
{
	if (m_mapCustRules == NULL)
		return 1.0;

	ZTSProductHotMap* prodMap = (ZTSProductHotMap*)m_mapCustRules->get(ruleTag);
	if (prodMap == NULL)
		return 1.0;

	ZTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), ZTSDateHotMap*);
	if (dtMap == NULL)
		return 1.0;

	if(uDate == 0)
	{
		ZTSSwitchItem* pItem = STATIC_CONVERT(dtMap->rbegin()->second, ZTSSwitchItem*);
		return pItem->get_factor();
	}

	auto it = dtMap->lower_bound(uDate);
	if(it == dtMap->end())
	{
		//找不到，说明记录的日期都比传入的日期小，所以返回最后一条的复权因子
		ZTSSwitchItem* pItem = STATIC_CONVERT(dtMap->rbegin()->second, ZTSSwitchItem*);
		return pItem->get_factor();
	}
	else
	{
		//找到了，就要看切换日期是否等于传入日期
		//如果相等，说明刚好切换，那么就直接返回复权因子
		ZTSSwitchItem* pItem = STATIC_CONVERT(it->second, ZTSSwitchItem*);
		if (pItem->switch_date() == uDate)
		{
			return pItem->get_factor();
		}
		else
		{
			//如果切换日期大于传入日期，则要看前一个阶段
			//如果已经是第一个了，则直接返回1.0
			if (it == dtMap->begin())
			{
				return 1.0;
			}
			else
			{
				//如果不是第一个，则回退一个，再返回即可
				it--;
				ZTSSwitchItem* pItem = STATIC_CONVERT(it->second, ZTSSwitchItem*);
				return pItem->get_factor();
			}
		}
	}

}

#pragma region "次主力接口"
bool ZTSHotMgr::loadHots(const char* filename)
{
	loadCustomRules("HOT", filename);
	m_bInitialized = true;
	return true;
}

const char* ZTSHotMgr::getPrevRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return getPrevCustomRawCode("HOT", fullPid, dt);
}

const char* ZTSHotMgr::getRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return getCustomRawCode("HOT", fullPid, dt);
}

bool ZTSHotMgr::isHot(const char* exchg, const char* rawCode, uint32_t dt)
{
	static thread_local char fullCode[64] = { 0 };
	fmtutil::format_to(fullCode, "{}.{}", exchg, rawCode);

	return isCustomHot("HOT", fullCode, dt);
}

bool ZTSHotMgr::splitHotSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return splitCustomSections("HOT", fullPid, sDt, eDt, sections);
}
#pragma endregion "主力接口"

#pragma region "次主力接口"
bool ZTSHotMgr::loadSeconds(const char* filename)
{
	return loadCustomRules("2ND", filename);
}

const char* ZTSHotMgr::getPrevSecondRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return getPrevCustomRawCode("2ND", fullPid, dt);
}

const char* ZTSHotMgr::getSecondRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return getCustomRawCode("2ND", fullPid, dt);
}

bool ZTSHotMgr::isSecond(const char* exchg, const char* rawCode, uint32_t dt)
{
	static thread_local char fullCode[64] = { 0 };
	fmtutil::format_to(fullCode, "{}.{}", exchg, rawCode);

	return isCustomHot("2NDT", fullCode, dt);
}

bool ZTSHotMgr::splitSecondSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return splitCustomSections("2ND", fullPid, sDt, eDt, sections);
}

#pragma endregion "次主力接口"

#pragma region "自定义主力接口"
bool ZTSHotMgr::loadCustomRules(const char* tag, const char* filename)
{
	if (!StdFile::exists(filename))
	{
		return false;
	}

	ZTSVariant* root = ZTSCfgLoader::load_from_file(filename);
	if (root == NULL)
		return false;

	if (m_mapCustRules == NULL)
		m_mapCustRules = ZTSCustomSwitchMap::create();

	ZTSProductHotMap* prodMap = (ZTSProductHotMap*)m_mapCustRules->get(tag);
	if(prodMap == NULL)
	{
		prodMap = ZTSProductHotMap::create();
		m_mapCustRules->add(tag, prodMap, false);
	}

	for (const std::string& exchg : root->memberNames())
	{
		ZTSVariant* jExchg = root->get(exchg);

		for (const std::string& pid : jExchg->memberNames())
		{
			ZTSVariant* jProduct = jExchg->get(pid);
			std::string fullPid = fmt::format("{}.{}", exchg, pid);

			ZTSDateHotMap* dateMap = ZTSDateHotMap::create();
			prodMap->add(fullPid.c_str(), dateMap, false);

			std::string lastCode;
			double factor = 1.0;
			for (uint32_t i = 0; i < jProduct->size(); i++)
			{
				ZTSVariant* jHotItem = jProduct->get(i);
				ZTSSwitchItem* pItem = ZTSSwitchItem::create(
					exchg.c_str(), pid.c_str(),
					jHotItem->getCString("from"), jHotItem->getCString("to"), 
					jHotItem->getUInt32("date"));

				//计算复权因子
				double oldclose = jHotItem->getDouble("oldclose");
				double newclose = jHotItem->getDouble("newclose");
				factor *= (decimal::eq(oldclose, 0.0) ? 1.0 : (oldclose/ newclose));
				pItem->set_factor(factor);
				dateMap->add(pItem->switch_date(), pItem, false);
				lastCode = jHotItem->getCString("to");
			}

			std::string fullCode = fmt::format("{}.{}", exchg.c_str(), lastCode.c_str());
			m_mapCustCodes[tag].insert(fullCode);
		}
	}

	root->release();
	return true;
}

const char* ZTSHotMgr::getPrevCustomRawCode(const char* tag, const char* fullPid, uint32_t dt /* = 0 */)
{
	if (m_mapCustRules == NULL)
		return "";

	if (dt == 0)
		dt = TimeUtils::getCurDate();

	if (m_mapCustRules == NULL)
		return "";

	ZTSProductHotMap* prodMap = (ZTSProductHotMap*)m_mapCustRules->get(tag);
	if (prodMap == NULL)
		return "";

	ZTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), ZTSDateHotMap*);
	if (dtMap == NULL)
		return "";

	ZTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if (cit != dtMap->end())
	{
		if (dt < cit->first)
			cit--;

		if (cit == dtMap->end() || cit == dtMap->begin())
			return "";

		cit--;

		ZTSSwitchItem* pItem = STATIC_CONVERT(cit->second, ZTSSwitchItem*);
		return pItem->to();
	}
	else
	{
		cit--;

		if (cit == dtMap->end() || cit == dtMap->begin())
			return "";

		cit--;

		ZTSSwitchItem* pItem = STATIC_CONVERT(cit->second, ZTSSwitchItem*);
		return pItem->to();
	}

	return "";
}

const char* ZTSHotMgr::getCustomRawCode(const char* tag, const char* fullPid, uint32_t dt /* = 0 */)
{
	if (m_mapCustRules == NULL)
		return "";

	if (dt == 0)
		dt = TimeUtils::getCurDate();

	ZTSProductHotMap* prodMap = (ZTSProductHotMap*)m_mapCustRules->get(tag);
	if (prodMap == NULL)
		return "";

	ZTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), ZTSDateHotMap*);
	if (dtMap == NULL)
		return "";

	ZTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if (cit != dtMap->end())
	{
		if (dt < cit->first)
			cit--;

		if (cit == dtMap->end())
			return "";

		ZTSSwitchItem* pItem = STATIC_CONVERT(cit->second, ZTSSwitchItem*);
		return pItem->to();
	}
	else
	{
		ZTSSwitchItem* pItem = STATIC_CONVERT(dtMap->last(), ZTSSwitchItem*);
		return pItem->to();
	}

	return "";
}

bool ZTSHotMgr::isCustomHot(const char* tag, const char* fullCode, uint32_t dt /* = 0 */)
{
	if (m_mapCustRules == NULL)
		return false;

	const auto& curHotCodes = m_mapCustCodes[tag];
	if (curHotCodes.empty())
		return false;

	if (dt == 0)
	{
		auto it = curHotCodes.find(fullCode);
		if (it == curHotCodes.end())
			return false;
		else
			return true;
	}

	auto idx = StrUtil::findFirst(fullCode, '.');
	const char* rawCode = fullCode + idx + 1;
	std::string fullPid(fullCode, idx);
	fullPid += ".";
	fullPid += CodeHelper::rawMonthCodeToRawCommID(rawCode);

	ZTSProductHotMap* prodMap = (ZTSProductHotMap*)m_mapCustRules->get(tag);
	if (prodMap == NULL)
		return "";

	ZTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), ZTSDateHotMap*);
	if (dtMap == NULL)
		return "";

	ZTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if (cit != dtMap->end())
	{
		ZTSSwitchItem* pItem = STATIC_CONVERT(cit->second, ZTSSwitchItem*);
		//因为登记的换月日期是开始生效的交易日，如果是下午盘后确定主力的话
		//那么dt就会是第二天，所以，dt必须大于等于切换日期
		if (pItem->switch_date() > dt)
			cit--;

		pItem = STATIC_CONVERT(cit->second, ZTSSwitchItem*);
		if (strcmp(pItem->to(), rawCode) == 0)
			return true;
	}
	else if (dtMap->size() > 0)
	{
		ZTSSwitchItem* pItem = (ZTSSwitchItem*)dtMap->last();
		if (strcmp(pItem->to(), rawCode) == 0)
			return true;
	}

	return false;
}

bool ZTSHotMgr::splitCustomSections(const char* tag, const char* fullPid, uint32_t sDt, uint32_t eDt, HotSections& sections)
{
	if (m_mapCustRules == NULL)
		return false;

	ZTSProductHotMap* prodMap = (ZTSProductHotMap*)m_mapCustRules->get(tag);
	if (prodMap == NULL)
		return false;

	ZTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), ZTSDateHotMap*);
	if (dtMap == NULL)
		return false;

	uint32_t leftDate = sDt;
	uint32_t lastDate = 0;
	const char* curHot = "";
	auto cit = dtMap->begin();
	double prevFactor = 1.0;
	for (; cit != dtMap->end(); cit++)
	{
		uint32_t curDate = cit->first;
		ZTSSwitchItem* hotItem = (ZTSSwitchItem*)cit->second;

		if (curDate > eDt)
		{
			sections.emplace_back(HotSection(hotItem->from(), leftDate, eDt, prevFactor));
		}
		else if (leftDate < curDate)
		{
			//如果开始日期小于当前切换的日期,则添加一段
			if (strlen(hotItem->from()) > 0)//这里from为空,主要是第一条规则,如果真的遇到这种情况,也没有太好的办法,只能不要这一段数据了,一般情况下是够的
			{
				sections.emplace_back(HotSection(hotItem->from(), leftDate, TimeUtils::getNextDate(curDate, -1), prevFactor));
			}

			leftDate = curDate;
		}

		lastDate = curDate;
		prevFactor = hotItem->get_factor();
		curHot = hotItem->to();
	}

	if (leftDate >= lastDate && lastDate != 0)
	{
		sections.emplace_back(HotSection(curHot, leftDate, eDt, prevFactor));
	}

	return true;
}
#pragma endregion "自定义主力接口"

void ZTSHotMgr::release()
{
	//if (m_pExchgHotMap)
	//{
	//	m_pExchgHotMap->release();
	//	m_pExchgHotMap = NULL;
	//}

	//if (m_pExchgScndMap)
	//{
	//	m_pExchgScndMap->release();
	//	m_pExchgScndMap = NULL;
	//}

	if(m_mapCustRules)
	{
		m_mapCustRules->release();
		m_mapCustRules = NULL;
	}
}