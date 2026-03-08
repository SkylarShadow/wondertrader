#include "ExpDumper.h"
#include "ZtDtRunner.h"

extern ZtDtRunner& getRunner();

bool ExpDumper::dumpHisOrdQue(const char* stdCode, uint32_t uDate, ZTSOrdQueStruct* items, uint32_t count)
{
	return getRunner().dumpHisOrdQue(_id.c_str(), stdCode, uDate, items, count);
}

bool ExpDumper::dumpHisOrdDtl(const char* stdCode, uint32_t uDate, ZTSOrdDtlStruct* items, uint32_t count)
{
	return getRunner().dumpHisOrdDtl(_id.c_str(), stdCode, uDate, items, count);
}

bool ExpDumper::dumpHisTrans(const char* stdCode, uint32_t uDate, ZTSTransStruct* items, uint32_t count)
{
	return getRunner().dumpHisTrans(_id.c_str(), stdCode, uDate, items, count);
}

bool ExpDumper::dumpHisBars(const char* stdCode, const char* period, ZTSBarStruct* bars, uint32_t count)
{
	return getRunner().dumpHisBars(_id.c_str(), stdCode, period, bars, count);
}

bool ExpDumper::dumpHisTicks(const char* stdCode, uint32_t uDate, ZTSTickStruct* ticks, uint32_t count)
{
	return getRunner().dumpHisTicks(_id.c_str(), stdCode, uDate, ticks, count);
}
