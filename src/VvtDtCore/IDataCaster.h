#pragma once
#include "../Includes/VvTSMarcos.h"

NS_VVTP_BEGIN
class VvTSTickData;
class VvTSVariant;
class VvTSOrdDtlData;
class VvTSOrdQueData;
class VvTSTransData;

class IDataCaster
{
public:
	virtual void	broadcast(VvTSTickData* curTick) = 0;
	virtual void	broadcast(VvTSOrdQueData* curOrdQue){}
	virtual void	broadcast(VvTSOrdDtlData* curOrdDtl){}
	virtual void	broadcast(VvTSTransData* curTrans){}
};

NS_VVTP_END
