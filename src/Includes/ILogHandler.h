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
#include "ZTSMarcos.h"
#include "ZTSTypes.h"

NS_ZTP_BEGIN
class ILogHandler
{
public:
	virtual void handleLogAppend(ZTSLogLevel ll, const char* msg)	= 0;
};
NS_ZTP_END