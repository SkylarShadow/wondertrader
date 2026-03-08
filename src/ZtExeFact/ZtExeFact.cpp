/*!
 * \file ZtExeFact.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "ZtExeFact.h"

#include "ZtTWapExeUnit.h"
#include "ZtMinImpactExeUnit.h"
#include "ZtDiffMinImpactExeUnit.h"
#include "ZtStockMinImpactExeUnit.h"
#include "ZtVWapExeUnit.h"
#include "ZtStockVWapExeUnit.h"
const char* FACT_NAME = "ZtExeFact";

extern "C"
{
	EXPORT_FLAG IExecuterFact* createExecFact()
	{
		IExecuterFact* fact = new ZtExeFact();
		return fact;
	}

	EXPORT_FLAG void deleteExecFact(IExecuterFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
};


ZtExeFact::ZtExeFact()
{
}


ZtExeFact::~ZtExeFact()
{
}

const char* ZtExeFact::getName()
{
	return FACT_NAME;
}

void ZtExeFact::enumExeUnit(FuncEnumUnitCallback cb)
{
	cb(FACT_NAME, "ZtTWapExeUnit", false);
	cb(FACT_NAME, "ZtMinImpactExeUnit", true);
}

ExecuteUnit* ZtExeFact::createExeUnit(const char* name)
{
	if (strcmp(name, "ZtTWapExeUnit") == 0)
		return new ZtTWapExeUnit();
	else if (strcmp(name, "ZtMinImpactExeUnit") == 0)
		return new ZtMinImpactExeUnit();
	else if (strcmp(name, "ZtStockMinImpactExeUnit") == 0)
		return new ZtStockMinImpactExeUnit();
	else if (strcmp(name, "ZtVWapExeUnit") == 0)
		return  new ZtVWapExeUnit();
	else if (strcmp(name, "ZtStockVWapExeUnit") == 0)
		return new ZtStockVWapExeUnit();
	return NULL;
}

ExecuteUnit* ZtExeFact::createDiffExeUnit(const char* name)
{
	if (strcmp(name, "ZtDiffMinImpactExeUnit") == 0)
		return new ZtDiffMinImpactExeUnit();

	return NULL;
}

ExecuteUnit* ZtExeFact::createArbiExeUnit(const char* name)
{
	return NULL;
}

bool ZtExeFact::deleteExeUnit(ExecuteUnit* unit)
{
	if (unit == NULL)
		return true;

	if (strcmp(unit->getFactName(), FACT_NAME) != 0)
		return false;

	delete unit;
	return true;
}
