/*!
 * \file EventCaster.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief UDP广播对象定义
 */
#pragma once

#include <boost/asio/io_service.hpp>

#include "../Includes/VvTSMarcos.h"
#include "../Includes/VvTSObject.hpp"
#include "../Share/StdUtils.hpp"

typedef unsigned long(*FuncCreateMQServer)(const char*);
typedef void(*FuncDestroyMQServer)(unsigned long);
typedef void(*FundPublishMessage)(unsigned long, const char*, const char*, unsigned long);

typedef void(*FuncLogCallback)(unsigned long, const char*, bool);
typedef void(*FuncRegCallbacks)(FuncLogCallback);


NS_VVTP_BEGIN
class VvTSTradeInfo;
class VvTSOrderInfo;
class VvTSVariant;

class EventNotifier
{
public:
	EventNotifier();
	~EventNotifier();

private:
	void	tradeToJson(const char* trader, uint32_t localid, const char* stdCode, VvTSTradeInfo* trdInfo, std::string& output);
	void	orderToJson(const char* trader, uint32_t localid, const char* stdCode, VvTSOrderInfo* ordInfo, std::string& output);

public:
	bool	init(VvTSVariant* cfg);

	void	notify(const char* trader, uint32_t localid, const char* stdCode, VvTSTradeInfo* trdInfo);
	void	notify(const char* trader, uint32_t localid, const char* stdCode, VvTSOrderInfo* ordInfo);
	void	notify(const char* trader, const char* message);

	void	notify_log(const char* tag, const char* message);

	void	notify_event(const char* message);

private:
	std::string		_url;
	uint32_t		_mq_sid;
	FuncCreateMQServer	_creator;
	FuncDestroyMQServer	_remover;
	FundPublishMessage	_publisher;
	FuncRegCallbacks	_register;

	bool			_stopped;
	boost::asio::io_service		_asyncio;
	StdThreadPtr				_worker;
};

NS_VVTP_END