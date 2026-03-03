/*!
 * \file VvtRiskMonFact.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "VvtRiskMonFact.h"
#include "VvtSimpRiskMon.h"

const char* FACT_NAME = "VvtRiskMonFact";

extern "C"
{
	EXPORT_FLAG IRiskMonitorFact* createRiskMonFact()
	{
		IRiskMonitorFact* fact = new VvtRiskMonFact();
		return fact;
	}

	EXPORT_FLAG void deleteRiskMonFact(IRiskMonitorFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


VvtRiskMonFact::VvtRiskMonFact()
{
}


VvtRiskMonFact::~VvtRiskMonFact()
{
}

const char* VvtRiskMonFact::getName()
{
	return FACT_NAME;
}

void VvtRiskMonFact::enumRiskMonitors(FuncEnumRiskMonCallback cb)
{
	//cb(FACT_NAME, "WtSimpExeUnit", false);
	cb(FACT_NAME, "SimpleRiskMon", true);
}

VvtRiskMonitor* VvtRiskMonFact::createRiskMonotor(const char* name)
{
	if (strcmp(name, "SimpleRiskMon") == 0)
		return new VvtSimpleRiskMon();
	return NULL;
}

bool VvtRiskMonFact::deleteRiskMonotor(VvtRiskMonitor* unit)
{
	if (unit == NULL)
		return true;

	if (strcmp(unit->getFactName(), FACT_NAME) != 0)
		return false;

	delete unit;
	return true;
}
