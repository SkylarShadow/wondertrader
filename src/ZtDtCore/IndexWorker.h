#pragma once
#include "../Includes/ZTSMarcos.h"
#include "../Includes/ZTSStruct.h"
#include "../Includes/FasterDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/SpinMutex.hpp"

NS_ZTP_BEGIN
class ZTSVariant;
class ZTSTickData;
class IHotMgr;
class IBaseDataMgr;
class ZTSContractInfo;
NS_ZTP_END

USING_NS_ZTP;

class IndexFactory;

class IndexWorker
{
public:
	IndexWorker(IndexFactory* factor):_factor(factor), _stopped(false), _process(false) {}

public:
	bool	init(ZTSVariant* config);
	void	handle_quote(ZTSTickData* newTick);

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
	ZTSTickStruct	_cache;
	ZTSContractInfo*	_cInfo;

	typedef struct _WeightFactor
	{
		double			_weight;
		ZTSTickStruct	_tick;
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