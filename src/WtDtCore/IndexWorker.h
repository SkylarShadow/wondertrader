#pragma once
#include "../Includes/VVTSMarcos.h"
#include "../Includes/WTSStruct.h"
#include "../Includes/FasterDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/SpinMutex.hpp"

NS_VVTP_BEGIN
class VVTSVariant;
class WTSTickData;
class IHotMgr;
class IBaseDataMgr;
class WTSContractInfo;
NS_VVTP_END

USING_NS_VVTP;

class IndexFactory;

class IndexWorker
{
public:
	IndexWorker(IndexFactory* factor):_factor(factor), _stopped(false), _process(false) {}

public:
	bool	init(VVTSVariant* config);
	void	handle_quote(WTSTickData* newTick);

private:
	void	generate_tick();

protected:
	IndexFactory*	_factor;
	std::string		_exchg;
	std::string		_code;
	std::string		_trigger;
	uint32_t		_timeout;
	uint64_t		_recalc_time;
	double			_stand_scale;
	WTSTickStruct	_cache;
	WTSContractInfo*	_cInfo;

	typedef struct _WeightFactor
	{
		double			_weight;
		WTSTickStruct	_tick;
		_WeightFactor()
		{
			memset(this, 0, sizeof(_WeightFactor));
		}
	}WeightFactor;
	SpinMutex	_mtx_data;
	vvt_hashmap<std::string, WeightFactor>	_weight_scales;
	uint32_t	_weight_alg;

	StdThreadPtr	_thrd_trigger;
	StdUniqueMutex	_mtx_trigger;
	StdCondVariable	_cond_trigger;
	bool			_stopped;
	bool			_process;
};

typedef std::shared_ptr<IndexWorker> IndexWorkerPtr;