/*!
 * \file VvTSError.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt错误对象定义
 */
#pragma once
#include "VvTSObject.hpp"
#include "VvTSTypes.h"

NS_VVTP_BEGIN

class VvTSError : public VvTSObject
{
protected:
	VvTSError():m_errCode(WEC_NONE),m_strMsg(""){}
	virtual ~VvTSError(){}

public:
	static VvTSError* create(VvTSErrorCode ec, const char* errmsg)
	{
		VvTSError* pRet = new VvTSError;
		pRet->m_errCode = ec;
		pRet->m_strMsg = errmsg;

		return pRet;
	}

	const char*		getMessage() const{return m_strMsg.c_str();}
	VvTSErrorCode		getErrorCode() const{return m_errCode;}

protected:
	VvTSErrorCode		m_errCode;
	std::string		m_strMsg;
};


NS_VVTP_END