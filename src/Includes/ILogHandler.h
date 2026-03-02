/*!
 * \file ILogHandler.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 日志转发模块接口定义
 */
#pragma once
#include "VVTSMarcos.h"
#include "WTSTypes.h"

NS_VVTP_BEGIN
class ILogHandler
{
public:
	virtual void handleLogAppend(WTSLogLevel ll, const char* msg)	= 0;
};
NS_VVTP_END