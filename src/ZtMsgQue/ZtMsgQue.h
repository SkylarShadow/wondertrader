/*!
 * \file ZtMsgQue.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "PorterDefs.h"

#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG void		regiter_callbacks(FuncLogCallback cbLog);

	EXPORT_FLAG ZTUInt32	create_server(const char* url, bool confirm);
	EXPORT_FLAG void		destroy_server(ZTUInt32 id);
	EXPORT_FLAG void		publish_message(ZTUInt32 id, const char* topic, const char* data, ZTUInt32 dataLen);

	EXPORT_FLAG ZTUInt32	create_client(const char* url, FuncMQCallback cb);
	EXPORT_FLAG void		destroy_client(ZTUInt32 id);
	EXPORT_FLAG void		subscribe_topic(ZTUInt32 id, const char* topic);
	EXPORT_FLAG void		start_client(ZTUInt32 id);
#ifdef __cplusplus
}
#endif