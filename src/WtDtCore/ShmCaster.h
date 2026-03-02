#pragma once
#include "IDataCaster.h"
#include <stdint.h>
#include "../Includes/VvTSStruct.h"
#include "../Share/BoostMappingFile.hpp"

NS_VVTP_BEGIN
class VvTSVariant;
NS_VVTP_END

USING_NS_VVTP;

class ShmCaster : public IDataCaster
{
public:
#pragma pack(push, 8)
	typedef struct _DataItem
	{
		uint32_t	_type;	//数据类型， 0-tick,1-委托队列,2-逐笔委托,3-逐笔成交
		union
		{
			VvTSTickStruct	_tick;
			VvTSOrdQueStruct _queue;
			VvTSOrdDtlStruct	_order;
			VvTSTransStruct	_trans;
		};

		_DataItem() { memset(this, 0, sizeof(_DataItem)); }
	} DataItem;

	template <int N = 8*1024>
	struct _DataQueue
	{
		uint64_t	_capacity = N;
		volatile uint64_t	_readable;
		volatile uint64_t	_writable;
		uint32_t	_pid;
		DataItem	_items[N];

		_DataQueue() :_readable(UINT64_MAX), _writable(0), _pid(0) {}
	};

	typedef _DataQueue<8*1024>	CastQueue;

#pragma pack(pop)

public:
	ShmCaster():_queue(NULL), _inited(false){}

	bool	init(VvTSVariant* cfg);

	virtual void	broadcast(VvTSTickData* curTick) override;
	virtual void	broadcast(VvTSOrdQueData* curOrdQue) override;
	virtual void	broadcast(VvTSOrdDtlData* curOrdDtl) override;
	virtual void	broadcast(VvTSTransData* curTrans) override;

private:
	std::string		_path;
	typedef std::shared_ptr<BoostMappingFile> MappedFilePtr;
	MappedFilePtr	_mapfile;
	CastQueue*		_queue;
	bool			_inited;
};

