#pragma once
#include "../Includes/VvTSMarcos.h"
#include "../Includes/VvTSStruct.h"
#include "../Includes/FasterDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/SpinMutex.hpp"

NS_VVTP_BEGIN
class VvTSVariant;
class VvTSTickData;
class IHotMgr;
class IBaseDataMgr;
class VvTSContractInfo;
NS_VVTP_END

USING_NS_VVTP;

class IndexFactory;

class IndexWorker
{
public:
	IndexWorker(IndexFactory* factor):_factor(factor), _stopped(false), _process(false) {}

public:
	bool	init(VvTSVariant* config);
	void	handle_quote(VvTSTickData* newTick);

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
	VvTSTickStruct	_cache;
	VvTSContractInfo*	_cInfo;

	typedef struct _WeightFactor
	{
		double			_weight;
		VvTSTickStruct	_tick;
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