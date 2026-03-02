#pragma once
#include "../Includes/VVTSMarcos.h"

NS_VVTP_BEGIN
class WTSTickData;
class VVTSVariant;
class WTSOrdDtlData;
class WTSOrdQueData;
class WTSTransData;

class IDataCaster
{
public:
	virtual void	broadcast(WTSTickData* curTick) = 0;
	virtual void	broadcast(WTSOrdQueData* curOrdQue){}
	virtual void	broadcast(WTSOrdDtlData* curOrdDtl){}
	virtual void	broadcast(WTSTransData* curTrans){}
};

NS_VVTP_END
