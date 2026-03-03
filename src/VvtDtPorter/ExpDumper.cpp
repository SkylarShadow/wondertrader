#include "ExpDumper.h"
#include "VvtDtRunner.h"

extern VvtDtRunner& getRunner();

bool ExpDumper::dumpHisOrdQue(const char* stdCode, uint32_t uDate, VvTSOrdQueStruct* items, uint32_t count)
{
	return getRunner().dumpHisOrdQue(_id.c_str(), stdCode, uDate, items, count);
}

bool ExpDumper::dumpHisOrdDtl(const char* stdCode, uint32_t uDate, VvTSOrdDtlStruct* items, uint32_t count)
{
	return getRunner().dumpHisOrdDtl(_id.c_str(), stdCode, uDate, items, count);
}

bool ExpDumper::dumpHisTrans(const char* stdCode, uint32_t uDate, VvTSTransStruct* items, uint32_t count)
{
	return getRunner().dumpHisTrans(_id.c_str(), stdCode, uDate, items, count);
}

bool ExpDumper::dumpHisBars(const char* stdCode, const char* period, VvTSBarStruct* bars, uint32_t count)
{
	return getRunner().dumpHisBars(_id.c_str(), stdCode, period, bars, count);
}

bool ExpDumper::dumpHisTicks(const char* stdCode, uint32_t uDate, VvTSTickStruct* ticks, uint32_t count)
{
	return getRunner().dumpHisTicks(_id.c_str(), stdCode, uDate, ticks, count);
}
