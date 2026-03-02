/*!
 * \file TraderFemas.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#define WINVER			0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE		0x0501

#include <string>
#include <queue>
#include <unordered_map>
#include <stdint.h>
#include <atomic>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include "../Includes/VvTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Share/StdUtils.hpp"
#include "../Includes/VvTSVariant.hpp"

//Femas v3.02
#include "../API/Femas3.02/USTPFtdcTraderApi.h"

#include "../Share/DLLHelper.hpp"

USING_NS_VVTP;

typedef std::unordered_map<std::string, std::string>	StringMap;

class TraderFemas : public ITraderApi, public CUstpFtdcTraderSpi
{
public:
	TraderFemas();
	virtual ~TraderFemas();

public:
	typedef enum 
	{
		WS_NOTLOGIN,		//未登录
		WS_LOGINING,		//正在登录
		WS_LOGINED,			//已登录
		WS_LOGINFAILED,		//登录失败
		WS_CONFIRM_QRYED,
		WS_CONFIRMED,		//已确认
		WS_ALLREADY			//全部就绪
	} WrapperState;


private:
	void onInitialized();

	int authenticate();

	int doLogin();

//////////////////////////////////////////////////////////////////////////
//ITraderApi接口
public:
	virtual bool init(VvTSVariant* params);

	virtual void release();

	virtual void setTag(const char* tag){m_strTag = tag;}
	virtual const char* getTag(){return m_strTag.c_str();}

	virtual void registerSpi( ITraderSpi *listener );

	virtual bool makeEntrustID(char* buffer, int length);

	virtual void connect();

	virtual void disconnect();

	virtual bool isConnected();

	virtual int login(const char* user, const char* pass, const char* productInfo);

	virtual int logout();

	virtual int orderInsert(VvTSEntrust* eutrust);

	virtual int orderAction(VvTSEntrustAction* action);

	virtual int queryAccount();

	virtual int queryPositions();

	virtual int queryOrders();

	virtual int queryTrades();


//////////////////////////////////////////////////////////////////////////
//Femas交易接口实现
public:
	virtual void OnFrontConnected() override;

	virtual void OnQryFrontConnected() override;

	virtual void OnFrontDisconnected(int nReason) override;

	virtual void OnQryFrontDisconnected(int nReason) override;

	virtual void OnHeartBeatWarning(int nTimeLapse) override;

	virtual void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///查询前置系统用户登录应答
	virtual void OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///成交回报
	virtual void OnRtnTrade(CUstpFtdcTradeField *pTrade) override;

	///报单回报
	virtual void OnRtnOrder(CUstpFtdcOrderField *pOrder) override;

	///报单录入应答
	virtual void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///报单操作应答
	virtual void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///投资者持仓查询应答
	virtual void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo) override;

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo) override;

	virtual void OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) override;

protected:
	bool IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo);

	int wrapPriceType(VvTSPriceType priceType, bool isCFFEX = false);
	int wrapDirectionType(VvTSDirectionType dirType, VvTSOffsetType offType);
	int wrapOffsetType(VvTSOffsetType offType);
	int	wrapTimeCondition(VvTSTimeCondition timeCond);
	int wrapActionFlag(VvTSActionFlag actionFlag);

	const char* wrapExchg(const char* exchg);


	VvTSPriceType		wrapPriceType(TUstpFtdcPriceType priceType);
	VvTSDirectionType	wrapDirectionType(TUstpFtdcDirectionType dirType, TUstpFtdcOffsetFlagType offType);
	VvTSDirectionType	wrapPosDirection(TUstpFtdcDirectionType dirType);
	VvTSOffsetType		wrapOffsetType(TUstpFtdcOffsetFlagType offType);
	VvTSTimeCondition	wrapTimeCondition(TUstpFtdcTimeConditionType timeCond);
	VvTSOrderState		wrapOrderState(TUstpFtdcOrderStatusType orderState);
	
	VvTSOrderInfo*	makeOrderInfo(CUstpFtdcOrderField* orderField);
	VvTSEntrust*		makeEntrust(CUstpFtdcInputOrderField *entrustField);
	VvTSError*		makeError(CUstpFtdcRspInfoField* rspInfo);
	VvTSTradeInfo*	makeTradeRecord(CUstpFtdcTradeField *tradeField);


	uint32_t		genRequestID();
	uint32_t		genLocalOrdID();

	void			triggerQuery();

protected:
	std::string		m_strBroker;
	std::string		m_strFront;
	std::string		m_strQryFront;

	std::string		m_strUser;
	std::string		m_strPass;
	std::string		m_strAppID;
	std::string		m_strAuthCode;
	std::string		m_strFlowDir;

	std::string		m_strProduct;

	bool			m_bQuickStart;

	std::string		m_strTag;

	std::string		m_strUserName;

	ITraderSpi*	m_sink;
	uint64_t		m_uLastQryTime;

	long			m_lDate;

	std::atomic<uint32_t>		m_uLocalOrdID;		//报单引用
	std::string					m_strSessionID;

	WrapperState				m_wrapperState;

	CUstpFtdcTraderApi*			m_pUserAPI;
	std::atomic<uint32_t>		m_iRequestID;

	StringMap					m_mapOrderTag;

	VvTSArray*					m_ayPosition;
	VvTSArray*					m_ayTrades;
	VvTSArray*					m_ayOrders;
	VvTSArray*					m_ayPosDetail;
	
	IBaseDataMgr*				m_bdMgr;

	bool						m_bQryOnline;
	bool						m_bStopped;

	boost::asio::io_service		m_asyncIO;
	boost::asio::io_service::strand*	m_strandIO;
	StdThreadPtr				m_thrdWorker;

	typedef std::queue<CommonExecuter>	QueryQue;
	QueryQue				m_queQuery;
	bool					m_bInQuery;
	StdUniqueMutex		m_mtxQuery;
	uint64_t				m_lastQryTime;

	std::string		m_strModule;
	DllHandle		m_hInstCTP;
	typedef CUstpFtdcTraderApi* (*APICreator)(const char *);
	APICreator		m_funcCreator;
};

