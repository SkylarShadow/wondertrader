#pragma once
#include "../Includes/ZTSMarcos.h"

NS_ZTP_BEGIN
class ZTSTickData;
class ZTSVariant;
class ZTSOrdDtlData;
class ZTSOrdQueData;
class ZTSTransData;

class IDataCaster
{
public:
	virtual void	broadcast(ZTSTickData* curTick) = 0;
	virtual void	broadcast(ZTSOrdQueData* curOrdQue){}
	virtual void	broadcast(ZTSOrdDtlData* curOrdDtl){}
	virtual void	broadcast(ZTSTransData* curTrans){}
};

NS_ZTP_END
