/*!
 * \file ZTSError.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt错误对象定义
 */
#pragma once
#include "ZTSObject.hpp"
#include "ZTSTypes.h"

NS_ZTP_BEGIN

class ZTSError : public ZTSObject
{
protected:
	ZTSError():m_errCode(WEC_NONE),m_strMsg(""){}
	virtual ~ZTSError(){}

public:
	static ZTSError* create(ZTSErrorCode ec, const char* errmsg)
	{
		ZTSError* pRet = new ZTSError;
		pRet->m_errCode = ec;
		pRet->m_strMsg = errmsg;

		return pRet;
	}

	const char*		getMessage() const{return m_strMsg.c_str();}
	ZTSErrorCode		getErrorCode() const{return m_errCode;}

protected:
	ZTSErrorCode		m_errCode;
	std::string		m_strMsg;
};


NS_ZTP_END