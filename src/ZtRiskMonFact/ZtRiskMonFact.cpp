/*!
 * \file ZtRiskMonFact.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "ZtRiskMonFact.h"
#include "ZtSimpRiskMon.h"

const char* FACT_NAME = "ZtRiskMonFact";

extern "C"
{
	EXPORT_FLAG IRiskMonitorFact* createRiskMonFact()
	{
		IRiskMonitorFact* fact = new ZtRiskMonFact();
		return fact;
	}

	EXPORT_FLAG void deleteRiskMonFact(IRiskMonitorFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


ZtRiskMonFact::ZtRiskMonFact()
{
}


ZtRiskMonFact::~ZtRiskMonFact()
{
}

const char* ZtRiskMonFact::getName()
{
	return FACT_NAME;
}

void ZtRiskMonFact::enumRiskMonitors(FuncEnumRiskMonCallback cb)
{
	//cb(FACT_NAME, "WtSimpExeUnit", false);
	cb(FACT_NAME, "SimpleRiskMon", true);
}

ZtRiskMonitor* ZtRiskMonFact::createRiskMonotor(const char* name)
{
	if (strcmp(name, "SimpleRiskMon") == 0)
		return new ZtSimpleRiskMon();
	return NULL;
}

bool ZtRiskMonFact::deleteRiskMonotor(ZtRiskMonitor* unit)
{
	if (unit == NULL)
		return true;

	if (strcmp(unit->getFactName(), FACT_NAME) != 0)
		return false;

	delete unit;
	return true;
}
