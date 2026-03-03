/*!
 * \file VvtMsgQue.h
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

	EXPORT_FLAG VvTUInt32	create_server(const char* url, bool confirm);
	EXPORT_FLAG void		destroy_server(VvTUInt32 id);
	EXPORT_FLAG void		publish_message(VvTUInt32 id, const char* topic, const char* data, VvTUInt32 dataLen);

	EXPORT_FLAG VvTUInt32	create_client(const char* url, FuncMQCallback cb);
	EXPORT_FLAG void		destroy_client(VvTUInt32 id);
	EXPORT_FLAG void		subscribe_topic(VvTUInt32 id, const char* topic);
	EXPORT_FLAG void		start_client(VvTUInt32 id);
#ifdef __cplusplus
}
#endif