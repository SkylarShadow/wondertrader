/*!
 * \file VvTSTradeDef.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt交易数据对象定义,包括委托、订单、成交、持仓、资金、持仓明细等数据
 */
#pragma once
#include "VvTSObject.hpp"
#include "VvTSTypes.h"
#include "VvTSMarcos.h"

#include <time.h>
#include <string>
#include <map>
#include<chrono>

NS_VVTP_BEGIN
class VvTSContractInfo;

//////////////////////////////////////////////////////////////////////////
//委托数据结构,用户客户端向服务端发起
class VvTSEntrust : public VvTSPoolObject<VvTSEntrust>
{
public:
	VvTSEntrust()
		: m_iPrice(0)
		, m_dVolume(0)
		, m_businessType(BT_CASH)
		, m_direction(WDT_LONG)
		, m_priceType(WPT_ANYPRICE)
		, m_orderFlag(WOF_NOR)
		, m_offsetType(WOT_OPEN)
		, m_bIsNet(false)
		, m_bIsBuy(true)
		, m_pContract(NULL)
	{
	}

	virtual ~VvTSEntrust(){}

public:
	static inline VvTSEntrust* create(const char* code, double vol, double price, const char* exchg = "", VvTSBusinessType bType = BT_CASH) noexcept
	{
		VvTSEntrust* pRet = VvTSEntrust::allocate();
		if(pRet)
		{
			//vvt_strcpy(pRet->m_strExchg, exchg);
			//vvt_strcpy(pRet->m_strCode, code);

			auto len = strlen(exchg);
			memcpy(pRet->m_strExchg, exchg, len);
			pRet->m_strExchg[len] = 0;

			len = strlen(code);
			memcpy(pRet->m_strCode, code, len);
			pRet->m_strCode[len] = 0;

			pRet->m_dVolume = vol;
			pRet->m_iPrice = price;
			pRet->m_businessType = bType;
			return pRet;
		}

		return NULL;
	}

public:
	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept
	{
		vvt_strcpy(m_strExchg, exchg, len);
    }
	inline void setCode(const char* code, std::size_t len = 0) noexcept
	{
		vvt_strcpy(m_strCode, code, len);
    }

	inline void setDirection(VvTSDirectionType dType)noexcept {m_direction = dType;}
	inline void setPriceType(VvTSPriceType pType)noexcept {m_priceType = pType;}
	inline void setOrderFlag(VvTSOrderFlag oFlag)noexcept {m_orderFlag = oFlag;}
	inline void setOffsetType(VvTSOffsetType oType)noexcept {m_offsetType = oType;}

	inline VvTSDirectionType	getDirection() const noexcept {return m_direction;}
	inline VvTSPriceType		getPriceType() const noexcept {return m_priceType;}
	inline VvTSOrderFlag		getOrderFlag() const noexcept {return m_orderFlag;}
	inline VvTSOffsetType	getOffsetType() const noexcept {return m_offsetType;}

	inline void setBusinessType(VvTSBusinessType bType) noexcept { m_businessType = bType; }
	inline VvTSBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	inline void setVolume(double volume) noexcept { m_dVolume = volume; }
	inline void setPrice(double price) noexcept { m_iPrice = price; }

	inline double getVolume() const noexcept { return m_dVolume; }
	inline double getPrice() const noexcept { return m_iPrice; }

	inline const char* getCode() const noexcept { return m_strCode; }
	inline const char* getExchg() const  noexcept { return m_strExchg; }

	inline void setEntrustID(const char* eid) noexcept { vvt_strcpy(m_strEntrustID, eid); }
	inline const char* getEntrustID() const  noexcept { return m_strEntrustID; }
	inline char* getEntrustID() noexcept { return m_strEntrustID; }

	inline void setUserTag(const char* tag)  noexcept { vvt_strcpy(m_strUserTag, tag); }
	inline const char* getUserTag() const  noexcept { return m_strUserTag; }
	inline char* getUserTag()  noexcept { return m_strUserTag; }

	inline void setNetDirection(bool isBuy) noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	inline bool isNet() const  noexcept { return m_bIsNet; }
	inline bool isBuy() const  noexcept { return m_bIsBuy; }

	inline void setContractInfo(VvTSContractInfo* cInfo) noexcept { m_pContract = cInfo; }
	inline VvTSContractInfo* getContractInfo() const  noexcept { return m_pContract; }

protected:
	char			m_strExchg[MAX_EXCHANGE_LENGTH];
	char			m_strCode[MAX_INSTRUMENT_LENGTH];
	double			m_dVolume;
	double			m_iPrice;

	bool			m_bIsNet;
	bool			m_bIsBuy;

	VvTSDirectionType	m_direction;
	VvTSPriceType		m_priceType;
	VvTSOrderFlag		m_orderFlag;
	VvTSOffsetType		m_offsetType;

	char				m_strEntrustID[64] = { 0 };
	char				m_strUserTag[64] = { 0 };

	VvTSBusinessType		m_businessType;

	VvTSContractInfo*	m_pContract;
};


//////////////////////////////////////////////////////////////////////////
//委托操作: 撤单、改单
class VvTSEntrustAction : public VvTSPoolObject<VvTSEntrustAction>
{
public:
	VvTSEntrustAction()
		: m_actionFlag(WAF_CANCEL)
		, m_businessType(BT_CASH)
	{

	}

	virtual ~VvTSEntrustAction(){}

public:
	static inline VvTSEntrustAction* create(const char* code, const char* exchg = "", VvTSBusinessType bType = BT_CASH) noexcept
	{
		VvTSEntrustAction* pRet = VvTSEntrustAction::allocate();
		if(pRet)
		{
			vvt_strcpy(pRet->m_strExchg, exchg);
			vvt_strcpy(pRet->m_strCode, code);
			pRet->m_businessType = bType;
			return pRet;
		}

		return NULL;
	}

	static inline VvTSEntrustAction* createCancelAction(const char* eid, const char* oid) noexcept
	{
		VvTSEntrustAction* pRet = new VvTSEntrustAction;
		if(pRet)
		{
			vvt_strcpy(pRet->m_strEnturstID, eid);
			vvt_strcpy(pRet->m_strOrderID, oid);
			return pRet;
		}

		return NULL;
	}

public:
	inline const char* getExchg() const  noexcept { return m_strExchg; }
	inline const char* getCode() const noexcept {return m_strCode;}

	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept {
		if (len == 0)
			vvt_strcpy(m_strExchg, exchg);
		else
			strncpy(m_strExchg, exchg, len);
	}
	inline void setCode(const char* code, std::size_t len = 0) noexcept {
		if (len == 0)
			vvt_strcpy(m_strCode, code);
		else
			strncpy(m_strCode, code, len);
	}

	inline void setActionFlag(VvTSActionFlag af) noexcept {m_actionFlag = af;}
	inline VvTSActionFlag getActionFlag() const noexcept {return m_actionFlag;}

	inline void setEntrustID(const char* eid) noexcept { vvt_strcpy(m_strEnturstID, eid); }
	inline const char* getEntrustID() const noexcept {return m_strEnturstID;}
	inline char* getEntrustID() noexcept { return m_strEnturstID; }

	inline void setOrderID(const char* oid) noexcept { vvt_strcpy(m_strOrderID, oid); }
	inline const char* getOrderID() const noexcept {return m_strOrderID;}

	inline void setBusinessType(VvTSBusinessType bType) noexcept { m_businessType = bType; }
	inline VvTSBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	inline void setUserTag(const char* tag)  noexcept { vvt_strcpy(m_strUserTag, tag); }
	inline const char* getUserTag() const  noexcept { return m_strUserTag; }
	inline char* getUserTag()  noexcept { return m_strUserTag; }

	inline void setContractInfo(VvTSContractInfo* cInfo)  noexcept { m_pContract = cInfo; }
	inline VvTSContractInfo* getContractInfo() const  noexcept { return m_pContract; }

protected:
	char			m_strExchg[MAX_EXCHANGE_LENGTH];
	char			m_strCode[MAX_INSTRUMENT_LENGTH];

	char			m_strEnturstID[64] = { 0 };
	VvTSActionFlag	m_actionFlag;

	char			m_strOrderID[64] = { 0 };
	char			m_strUserTag[64] = { 0 };

	VvTSBusinessType		m_businessType;
	VvTSContractInfo*	m_pContract;
};

//////////////////////////////////////////////////////////////////////////
//订单信息,查看订单状态变化等
class VvTSOrderInfo : public VvTSPoolObject<VvTSOrderInfo>
{
public:
	VvTSOrderInfo()
		:m_orderState(WOS_Submitting)
		,m_orderType(WORT_Normal)
		,m_uInsertDate(0)
		,m_uInsertTime(0)
		,m_dVolTraded(0)
		,m_dVolLeft(0)
		,m_bIsError(false)
	{

	}

	virtual ~VvTSOrderInfo(){}

public:
	static inline VvTSOrderInfo* create(VvTSEntrust* entrust = NULL) noexcept
	{
		VvTSOrderInfo *pRet = VvTSOrderInfo::allocate();

		if(entrust != NULL)
		{
			vvt_strcpy(pRet->m_strCode, entrust->getCode());
			vvt_strcpy(pRet->m_strExchg,entrust->getExchg());
			pRet->m_iPrice = entrust->getPrice();
			pRet->m_dVolume = entrust->getVolume();

			pRet->m_direction = entrust->getDirection();
			pRet->m_offsetType = entrust->getOffsetType();
			pRet->m_orderFlag = entrust->getOrderFlag();
			pRet->m_priceType = entrust->getPriceType();
			vvt_strcpy(pRet->m_strEntrustID, entrust->getEntrustID());
			vvt_strcpy(pRet->m_strUserTag, entrust->getUserTag());

			pRet->m_dVolLeft = entrust->getVolume();
			pRet->m_businessType = entrust->getBusinessType();
		}

		return pRet;
	}

public:
	//这部分是和VvTSEntrust同步的
	inline void setExchange(const char* exchg, std::size_t len = 0)  noexcept {
		if (len == 0)
			vvt_strcpy(m_strExchg, exchg);
		else
			strncpy(m_strExchg, exchg, len);
	}
	inline void setCode(const char* code, std::size_t len = 0) noexcept {
		if (len == 0)
			vvt_strcpy(m_strCode, code);
		else
			strncpy(m_strCode, code, len);
	}

	inline void setDirection(VvTSDirectionType dType)  noexcept { m_direction = dType; }
	inline void setPriceType(VvTSPriceType pType)  noexcept { m_priceType = pType; }
	inline void setOrderFlag(VvTSOrderFlag oFlag)  noexcept { m_orderFlag = oFlag; }
	inline void setOffsetType(VvTSOffsetType oType) noexcept { m_offsetType = oType; }

	inline VvTSDirectionType	getDirection() const  noexcept { return m_direction; }
	inline VvTSPriceType		getPriceType() const  noexcept { return m_priceType; }
	inline VvTSOrderFlag		getOrderFlag() const  noexcept { return m_orderFlag; }
	inline VvTSOffsetType	getOffsetType() const  noexcept { return m_offsetType; }

	inline void setBusinessType(VvTSBusinessType bType)  noexcept { m_businessType = bType; }
	inline VvTSBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	inline void setVolume(double volume)  noexcept { m_dVolume = volume; }
	inline void setPrice(double price)  noexcept { m_iPrice = price; }

	inline double getVolume() const  noexcept { return m_dVolume; }
	inline double getPrice() const  noexcept { return m_iPrice; }

	inline const char* getCode() const noexcept { return m_strCode; }
	inline const char* getExchg() const  noexcept { return m_strExchg; }

	inline void setEntrustID(const char* eid)  noexcept { vvt_strcpy(m_strEntrustID, eid); }
	inline const char* getEntrustID() const  noexcept { return m_strEntrustID; }
	inline char* getEntrustID()  noexcept { return m_strEntrustID; }

	inline void setUserTag(const char* tag)  noexcept { vvt_strcpy(m_strUserTag, tag); }
	inline const char* getUserTag() const  noexcept { return m_strUserTag; }
	inline char* getUserTag() noexcept { return m_strUserTag; }

	inline void setNetDirection(bool isBuy)  noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	inline bool isNet() const  noexcept { return m_bIsNet; }
	inline bool isBuy() const  noexcept { return m_bIsBuy; }

	inline void setContractInfo(VvTSContractInfo* cInfo)  noexcept { m_pContract = cInfo; }
	inline VvTSContractInfo* getContractInfo() const  noexcept { return m_pContract; }

public:
	inline void	setOrderDate(uint32_t uDate) noexcept {m_uInsertDate = uDate;}
	inline void	setOrderTime(uint64_t uTime) noexcept {m_uInsertTime = uTime;}
	inline void	setVolTraded(double vol) noexcept { m_dVolTraded = vol; }
	inline void	setVolLeft(double vol) noexcept { m_dVolLeft = vol; }
	
	inline void	setOrderID(const char* oid) noexcept { vvt_strcpy(m_strOrderID, oid); }
	inline void	setOrderState(VvTSOrderState os) noexcept {m_orderState = os;}
	inline void	setOrderType(VvTSOrderType ot) noexcept {m_orderType = ot;}

	inline uint32_t getOrderDate() const noexcept {return m_uInsertDate;}
	inline uint64_t getOrderTime() const noexcept {return m_uInsertTime;}
	inline double getVolTraded() const noexcept { return m_dVolTraded; }
	inline double getVolLeft() const noexcept { return m_dVolLeft; }
    
	inline VvTSOrderState		getOrderState() const  noexcept { return m_orderState; }
	inline VvTSOrderType			getOrderType() const  noexcept { return m_orderType; }
	inline const char*			getOrderID() const  noexcept { return m_strOrderID; }
	inline char*			getOrderID()  noexcept { return m_strOrderID; }

	inline void	setStateMsg(const char* msg) noexcept {m_strStateMsg = msg;}
	inline const char* getStateMsg() const noexcept {return m_strStateMsg.c_str();}

	inline bool	isAlive() const noexcept
	{
		if (m_bIsError)
			return false;

		switch(m_orderState)
		{
		case WOS_AllTraded:
		case WOS_Canceled:
			return false;
		default:
			return true;
		}
	}

	inline void	setError(bool bError = true) noexcept { m_bIsError = bError; }
	inline bool	isError() const noexcept { return m_bIsError; }

private:
	//这部分成员和VvTSEntrust一致
	char			m_strExchg[MAX_EXCHANGE_LENGTH];
	char			m_strCode[MAX_INSTRUMENT_LENGTH];
	double			m_dVolume;
	double			m_iPrice;

	bool			m_bIsNet;
	bool			m_bIsBuy;

	VvTSDirectionType	m_direction;
	VvTSPriceType		m_priceType;
	VvTSOrderFlag		m_orderFlag;
	VvTSOffsetType		m_offsetType;

	char				m_strEntrustID[64] = { 0 };
	char				m_strUserTag[64] = { 0 };

	VvTSBusinessType		m_businessType;
	VvTSContractInfo*	m_pContract;

	//这部分是Order专有的成员
	uint32_t	m_uInsertDate;
	uint64_t	m_uInsertTime;
	double		m_dVolTraded;
	double		m_dVolLeft;
	bool		m_bIsError;

	VvTSOrderState	m_orderState;
	VvTSOrderType	m_orderType;
	char			m_strOrderID[64] = { 0 };
	std::string		m_strStateMsg;
};

//////////////////////////////////////////////////////////////////////////
class VvTSTradeInfo : public VvTSPoolObject<VvTSTradeInfo>
{
public:
	VvTSTradeInfo()
		: m_orderType(WORT_Normal)
		, m_tradeType(VvTT_Common)
		, m_uAmount(0)
		, m_dPrice(0)
		, m_businessType(BT_CASH)
		, m_pContract(NULL)
	{}
	virtual ~VvTSTradeInfo(){}

public:
	static inline VvTSTradeInfo* create(const char* code, const char* exchg = "", VvTSBusinessType bType = BT_CASH)
	{
		VvTSTradeInfo *pRet = VvTSTradeInfo::allocate();
		vvt_strcpy(pRet->m_strExchg, exchg);
		vvt_strcpy(pRet->m_strCode, code);
		pRet->m_businessType = bType;

		return pRet;
	}

	inline void setTradeID(const char* tradeid) noexcept { vvt_strcpy(m_strTradeID, tradeid); }
	inline void setRefOrder(const char* oid) noexcept { vvt_strcpy(m_strRefOrder, oid); }
	
	inline void setDirection(VvTSDirectionType dType)noexcept {m_direction = dType;}
	inline void setOffsetType(VvTSOffsetType oType)noexcept {m_offsetType = oType;}
	inline void setOrderType(VvTSOrderType ot)noexcept {m_orderType = ot;}
	inline void setTradeType(VvTSTradeType tt)noexcept {m_tradeType = tt;}

	inline void setVolume(double volume)noexcept {m_dVolume = volume;}
	inline void setPrice(double price)noexcept { m_dPrice = price; }

	inline void setTradeDate(uint32_t uDate)noexcept {m_uTradeDate = uDate;}
	inline void setTradeTime(uint64_t uTime)noexcept {m_uTradeTime = uTime;}

	inline void setAmount(double amount) noexcept { m_uAmount = amount; }

	inline VvTSDirectionType	getDirection() const noexcept {return m_direction;}
	inline VvTSOffsetType	getOffsetType() const noexcept {return m_offsetType;}
	inline VvTSOrderType		getOrderType() const noexcept {return m_orderType;}
	inline VvTSTradeType		getTradeType() const noexcept {return m_tradeType;}

	inline double getVolume() const { return m_dVolume; }
	inline double getPrice() const noexcept { return m_dPrice; }

	inline const char*	getCode() const noexcept { return m_strCode; }
	inline const char*	getExchg() const noexcept { return m_strExchg; }
	inline const char*	getTradeID() const noexcept { return m_strTradeID; }
	inline const char*	getRefOrder() const noexcept { return m_strRefOrder; }

	inline char*	getTradeID() noexcept { return m_strTradeID; }
	inline char*	getRefOrder() noexcept { return m_strRefOrder; }

	inline uint32_t getTradeDate() const noexcept {return m_uTradeDate;}
	inline uint64_t getTradeTime() const noexcept {return m_uTradeTime;}

	inline double getAmount() const noexcept { return m_uAmount; }

	inline void setUserTag(const char* tag) noexcept { vvt_strcpy(m_strUserTag, tag); }
	inline const char* getUserTag() const noexcept { return m_strUserTag; }

	inline void setBusinessType(VvTSBusinessType bType) noexcept  { m_businessType = bType; }
	inline VvTSBusinessType	getBusinessType() const noexcept { return m_businessType; }

	inline void setNetDirection(bool isBuy) noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	inline bool isNet() const noexcept { return m_bIsNet; }
	inline bool isBuy() const noexcept { return m_bIsBuy; }

	inline void setContractInfo(VvTSContractInfo* cInfo) noexcept { m_pContract = cInfo; }
	inline VvTSContractInfo* getContractInfo() const noexcept { return m_pContract; }

protected:
	char	m_strExchg[MAX_EXCHANGE_LENGTH];	//市场
	char	m_strCode[MAX_INSTRUMENT_LENGTH];	//代码
	char	m_strTradeID[64] = { 0 };			//成交单号
	char	m_strRefOrder[64] = { 0 };			//本地委托序列号
	char	m_strUserTag[64] = { 0 };			//用户标签

	uint32_t	m_uTradeDate;
	uint64_t	m_uTradeTime;
	double		m_dVolume;
	double		m_dPrice;

	bool		m_bIsNet;
	bool		m_bIsBuy;

	VvTSDirectionType	m_direction;
	VvTSOffsetType		m_offsetType;
	VvTSOrderType		m_orderType;
	VvTSTradeType		m_tradeType;

	double		m_uAmount;

	VvTSBusinessType		m_businessType;
	VvTSContractInfo*	m_pContract;
};

//////////////////////////////////////////////////////////////////////////
//持仓信息
class VvTSPositionItem : public VvTSPoolObject<VvTSPositionItem>
{
public:
	static inline VvTSPositionItem* create(const char* code, const char* currency = "CNY", const char* exchg = "", VvTSBusinessType bType = BT_CASH)
	{
		VvTSPositionItem *pRet = VvTSPositionItem::allocate();
		vvt_strcpy(pRet->m_strExchg, exchg);
		vvt_strcpy(pRet->m_strCode, code);
		vvt_strcpy(pRet->m_strCurrency, currency);
		pRet->m_businessType = bType;

		return pRet;
	}

	inline void setDirection(VvTSDirectionType dType){m_direction = dType;}
	inline void setPrePosition(double prePos){ m_dPrePosition = prePos; }
	inline void setNewPosition(double newPos){ m_dNewPosition = newPos; }
	inline void setAvailPrePos(double availPos){ m_dAvailPrePos = availPos; }
	inline void setAvailNewPos(double availPos){ m_dAvailNewPos = availPos; }
	inline void setPositionCost(double cost){m_dTotalPosCost = cost;}
	inline void setMargin(double margin){ m_dMargin = margin; }
	inline void setAvgPrice(double avgPrice){ m_dAvgPrice = avgPrice; }
	inline void setDynProfit(double profit){ m_dDynProfit = profit; }

	inline VvTSDirectionType getDirection() const{return m_direction;}
	inline double	getPrePosition() const{ return m_dPrePosition; }
	inline double	getNewPosition() const{ return m_dNewPosition; }
	inline double	getAvailPrePos() const{ return m_dAvailPrePos; }
	inline double	getAvailNewPos() const{ return m_dAvailNewPos; }

	inline double	getTotalPosition() const{ return m_dPrePosition + m_dNewPosition; }
	inline double	getAvailPosition() const{ return m_dAvailPrePos + m_dAvailNewPos; }

	inline double	getFrozenPosition() const{ return getTotalPosition() - getAvailPosition(); }
	inline double	getFrozenNewPos() const{ return m_dNewPosition - m_dAvailNewPos; }
	inline double	getFrozenPrePos() const{ return m_dPrePosition - m_dAvailPrePos; }

	inline double		getPositionCost() const{ return m_dTotalPosCost; }
	inline double		getMargin() const{ return m_dMargin; }
	inline double		getAvgPrice() const{ return m_dAvgPrice; }
	inline double		getDynProfit() const{ return m_dDynProfit; }

	inline const char* getCode() const{ return m_strCode; }
	inline const char* getCurrency() const{ return m_strCurrency; }
	inline const char* getExchg() const{ return m_strExchg; }

	inline void setBusinessType(VvTSBusinessType bType) { m_businessType = bType; }
	inline VvTSBusinessType	getBusinessType() const { return m_businessType; }

	inline void setContractInfo(VvTSContractInfo* cInfo) { m_pContract = cInfo; }
	inline VvTSContractInfo* getContractInfo() const { return m_pContract; }

public:
	VvTSPositionItem()
		: m_direction(WDT_LONG)
		, m_dPrePosition(0)
		, m_dNewPosition(0)
		, m_dAvailPrePos(0)
		, m_dAvailNewPos(0)
		, m_dMargin(0)
		, m_dAvgPrice(0)
		, m_dDynProfit(0)
		, m_dTotalPosCost(0)
		, m_businessType(BT_CASH)
		, m_pContract(NULL)
	{}
	virtual ~VvTSPositionItem(){}

protected:
	char			m_strExchg[MAX_EXCHANGE_LENGTH];
	char			m_strCode[MAX_INSTRUMENT_LENGTH];
	char			m_strCurrency[8] = { 0 };

	VvTSDirectionType	m_direction;//多空方向
	double		m_dPrePosition;		//昨仓
	double		m_dNewPosition;		//今仓
	double		m_dAvailPrePos;		//可平昨仓
	double		m_dAvailNewPos;		//可平今仓
	double		m_dTotalPosCost;	//持仓总成本
	double		m_dMargin;			//占用保证金
	double		m_dAvgPrice;		//持仓均价
	double		m_dDynProfit;		//浮动盈亏

	VvTSBusinessType		m_businessType;
	VvTSContractInfo*	m_pContract;
};

//////////////////////////////////////////////////////////////////////////
//账户信息
class VvTSAccountInfo : public VvTSPoolObject<VvTSAccountInfo>
{
public:
	VvTSAccountInfo()
		: m_strCurrency("CNY")
		, m_dBalance(0)
		, m_dPreBalance(0)
		, m_dCommission(0)
		, m_dFrozenMargin(0)
		, m_dFrozenCommission(0)
		, m_dCloseProfit(0)
		, m_dDynProfit(0)
		, m_dDeposit(0)
		, m_dWithdraw(0)
		, m_dAvailable(0)
	{
	}
	virtual ~VvTSAccountInfo(){}

public:
	static inline VvTSAccountInfo* create(){return VvTSAccountInfo::allocate();}

	inline void	setCurrency(const char* currency){ m_strCurrency = currency; }

	inline void	setBalance(double balance){m_dBalance = balance;}
	inline void	setPreBalance(double prebalance){m_dPreBalance = prebalance;}
	inline void	setMargin(double margin){m_uMargin = margin;}
	inline void	setFrozenMargin(double frozenmargin){m_dFrozenMargin = frozenmargin;}
	inline void	setCloseProfit(double closeprofit){m_dCloseProfit = closeprofit;}
	inline void	setDynProfit(double dynprofit){m_dDynProfit = dynprofit;}
	inline void	setDeposit(double deposit){m_dDeposit = deposit;}
	inline void	setWithdraw(double withdraw){m_dWithdraw = withdraw;}
	inline void	setCommission(double commission){m_dCommission = commission;}
	inline void	setFrozenCommission(double frozencommission){m_dFrozenCommission = frozencommission;}
	inline void	setAvailable(double available){m_dAvailable = available;}

	inline double	getBalance() const{return m_dBalance;}
	inline double	getPreBalance() const{return m_dPreBalance;}
	inline double	getMargin() const{return m_uMargin;}
	inline double	getFrozenMargin() const{return m_dFrozenMargin;}
	inline double	getCloseProfit() const{return m_dCloseProfit;}
	inline double	getDynProfit() const{return m_dDynProfit;}
	inline double	getDeposit() const{return m_dDeposit;}
	inline double	getWithdraw() const{return m_dWithdraw;}
	inline double	getCommission() const{return m_dCommission;}
	inline double	getFrozenCommission() const{return m_dFrozenCommission;}
	inline double	getAvailable() const{return m_dAvailable;}

	inline const char* getCurrency() const{ return m_strCurrency.c_str(); }

protected:
	std::string m_strCurrency;

	double		m_dBalance;					//余额
	double		m_dPreBalance;				//上次结算准备金
	double		m_uMargin;					//当前保证金总额
	double		m_dCommission;				//手续费
	double		m_dFrozenMargin;			//冻结的保证金
	double		m_dFrozenCommission;		//冻结的手续费
	double		m_dCloseProfit;				//平仓盈亏
	double		m_dDynProfit;				//持仓盈亏
	double		m_dDeposit;					//入金金额
	double		m_dWithdraw;				//出金金额
	double		m_dAvailable;				//可用资金
};


NS_VVTP_END
