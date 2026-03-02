/*!
 * \file TraderOES.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include <stdint.h>
#include <boost/asio/io_service.hpp>

#include <oes_api/oes_async_api.h>

#include "../Includes/ITraderApi.h"
#include "../Includes/VvTSCollection.hpp"

#include "../Share/VvtKVCache.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

USING_NS_VVTP;

class TraderOES : public ITraderApi
{
public:
	TraderOES();
	virtual ~TraderOES();

	typedef enum
	{
		TS_NOTLOGIN,		//未登录
		TS_LOGINING,		//正在登录
		TS_LOGINED,			//已登录
		TS_LOGINFAILED,		//登录失败
		TS_ALLREADY			//全部就绪
	} TraderState;

public:
	void	handle_ord_connected(OesAsyncApiChannelT *pAsyncChannel);
	void	handle_ord_disconnected(OesAsyncApiChannelT *pAsyncChannel);
	void	handle_rpt_connected(OesAsyncApiChannelT *pAsyncChannel);
	void	handle_rpt_disconnected(OesAsyncApiChannelT *pAsyncChannel);
	void	handle_rpt_message(SMsgHeadT *pMsgHead, void *pMsgItem);
	void	handle_ord_message(SMsgHeadT *pMsgHead, void *pMsgItem);

	void	handle_rsp_account(SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor);
	void	handle_rsp_orders(SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor);
	void	handle_rsp_trades(SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor);
	void	handle_rsp_positions(SMsgHeadT *pMsgHead, void *pMsgItem, OesQryCursorT *pQryCursor);

public:
	//////////////////////////////////////////////////////////////////////////
	//ITraderApi 接口
	virtual bool init(VvTSVariant *params) override;

	virtual void release() override;

	virtual void registerSpi(ITraderSpi *listener) override;

	virtual void connect() override;

	virtual void disconnect() override;

	virtual bool isConnected() override;

	virtual bool makeEntrustID(char* buffer, int length) override;

	virtual int login(const char* user, const char* pass, const char* productInfo) override;

	virtual int logout() override;

	virtual int orderInsert(VvTSEntrust* eutrust) override;

	virtual int orderAction(VvTSEntrustAction* action) override;

	virtual int queryAccount() override;

	virtual int queryPositions() override;

	virtual int queryOrders() override;

	virtual int queryTrades() override;

private:
	void		reconnect();
	void		doLogin();

	inline VvTSOrderInfo*	makeOrderInfo(OesOrdCnfmT* orderField);
	inline VvTSEntrust*		makeEntrust(OesOrdRejectT *entrustField);
	inline VvTSTradeInfo*	makeTradeInfo(OesTrdCnfmT *tradeField);

	inline bool	extractEntrustID(const char* entrustid, uint32_t &orderRef);
	inline void	genEntrustID(char* buffer, uint32_t orderRef);

private:
	OesAsyncApiContextT*	_context;
	OesAsyncApiChannelT*	_channel;
	OesAsyncApiChannelT*	_channel_qry;
	ITraderSpi*				_sink;

	IBaseDataMgr*			_bd_mgr;

	VvTSArray*		_trades;
	VvTSArray*		_orders;
	VvTSArray*		_funds;
	VvTSArray*		_positions;

	std::string		_config;
	std::string		_user;
	bool			_inited;

	TraderState		_state;

	uint32_t		_tradingday;

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	//委托单标记缓存器
	VvtKVCache		m_eidCache;
	//订单标记缓存器
	VvtKVCache		m_oidCache;
};

