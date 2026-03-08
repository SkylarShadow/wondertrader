/*!
 * \file ZTSHotMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 主力合约管理器实现
 */
#pragma once
#include "../Includes/IHotMgr.h"
#include "../Includes/FasterDefs.h"
#include "../Includes/ZTSCollection.hpp"
#include <string>

NS_ZTP_BEGIN
	class ZTSSwitchItem;
NS_ZTP_END

USING_NS_ZTP;

//换月主力映射
typedef ZTSMap<uint32_t>			ZTSDateHotMap;
//品种主力映射
typedef ZTSHashMap<std::string>		ZTSProductHotMap;
//分市场主力映射
typedef ZTSHashMap<std::string>		ZTSExchgHotMap;

//自定义切换规则映射
typedef ZTSHashMap<std::string>		ZTSCustomSwitchMap;

class ZTSHotMgr : public IHotMgr
{
public:
	ZTSHotMgr();
	~ZTSHotMgr();

public:
	bool loadHots(const char* filename);
	bool loadSeconds(const char* filename);
	void release();

	bool loadCustomRules(const char* tag, const char* filename);

	inline bool isInitialized() const {return m_bInitialized;}

public:
	virtual const char* getRuleTag(const char* stdCode) override;

	virtual double		getRuleFactor(const char* ruleTag, const char* fullPid, uint32_t uDate  = 0 ) override;

	//////////////////////////////////////////////////////////////////////////
	//主力接口
	virtual const char* getRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual const char* getPrevRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual bool	isHot(const char* exchg, const char* rawCode, uint32_t dt = 0) override;

	virtual bool	splitHotSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections) override;

	//////////////////////////////////////////////////////////////////////////
	//次主力接口
	virtual const char* getSecondRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual const char* getPrevSecondRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual bool		isSecond(const char* exchg, const char* rawCode, uint32_t dt = 0) override;

	virtual bool		splitSecondSecions(const char* exchg, const char* hotCode, uint32_t sDt, uint32_t eDt, HotSections& sections) override;

	//////////////////////////////////////////////////////////////////////////
	//通用接口
	virtual const char* getCustomRawCode(const char* tag, const char* fullPid, uint32_t dt) override;

	virtual const char* getPrevCustomRawCode(const char* tag, const char* fullPid, uint32_t dt) override;

	virtual bool		isCustomHot(const char* tag, const char* fullCode, uint32_t dt) override;

	virtual bool		splitCustomSections(const char* tag, const char* fullPid, uint32_t sDt, uint32_t eDt, HotSections& sections) override;


private:
	//ZTSExchgHotMap*	m_pExchgHotMap;
	//ZTSExchgHotMap*	m_pExchgScndMap;
	//vvt_hashset<std::string>	m_curHotCodes;
	//vvt_hashset<std::string>	m_curSecCodes;
	bool			m_bInitialized;

	ZTSCustomSwitchMap*	m_mapCustRules;
	typedef vvt_hashmap<std::string, vvt_hashset<std::string>>	CustomSwitchCodes;
	CustomSwitchCodes	m_mapCustCodes;
};

