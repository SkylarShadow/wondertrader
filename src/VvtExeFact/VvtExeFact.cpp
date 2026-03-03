/*!
 * \file VvtExeFact.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "VvtExeFact.h"

#include "VvtTWapExeUnit.h"
#include "VvtMinImpactExeUnit.h"
#include "VvtDiffMinImpactExeUnit.h"
#include "VvtStockMinImpactExeUnit.h"
#include "VvtVWapExeUnit.h"
#include "VvtStockVWapExeUnit.h"
const char* FACT_NAME = "VvtExeFact";

extern "C"
{
	EXPORT_FLAG IExecuterFact* createExecFact()
	{
		IExecuterFact* fact = new VvtExeFact();
		return fact;
	}

	EXPORT_FLAG void deleteExecFact(IExecuterFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
};


VvtExeFact::VvtExeFact()
{
}


VvtExeFact::~VvtExeFact()
{
}

const char* VvtExeFact::getName()
{
	return FACT_NAME;
}

void VvtExeFact::enumExeUnit(FuncEnumUnitCallback cb)
{
	cb(FACT_NAME, "VvtTWapExeUnit", false);
	cb(FACT_NAME, "VvtMinImpactExeUnit", true);
}

ExecuteUnit* VvtExeFact::createExeUnit(const char* name)
{
	if (strcmp(name, "VvtTWapExeUnit") == 0)
		return new VvtTWapExeUnit();
	else if (strcmp(name, "VvtMinImpactExeUnit") == 0)
		return new VvtMinImpactExeUnit();
	else if (strcmp(name, "VvtStockMinImpactExeUnit") == 0)
		return new VvtStockMinImpactExeUnit();
	else if (strcmp(name, "VvtVWapExeUnit") == 0)
		return  new VvtVWapExeUnit();
	else if (strcmp(name, "VvtStockVWapExeUnit") == 0)
		return new VvtStockVWapExeUnit();
	return NULL;
}

ExecuteUnit* VvtExeFact::createDiffExeUnit(const char* name)
{
	if (strcmp(name, "VvtDiffMinImpactExeUnit") == 0)
		return new VvtDiffMinImpactExeUnit();

	return NULL;
}

ExecuteUnit* VvtExeFact::createArbiExeUnit(const char* name)
{
	return NULL;
}

bool VvtExeFact::deleteExeUnit(ExecuteUnit* unit)
{
	if (unit == NULL)
		return true;

	if (strcmp(unit->getFactName(), FACT_NAME) != 0)
		return false;

	delete unit;
	return true;
}
