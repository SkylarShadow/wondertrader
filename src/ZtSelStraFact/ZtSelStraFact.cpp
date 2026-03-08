#include "ZtSelStraFact.h"
#include "ZtStraDtSel.h"

#include <string.h>
#include <boost/config.hpp>

const char* FACT_NAME = "ZtSelStraFact";


extern "C"
{
	EXPORT_FLAG ISelStrategyFact* createSelStrategyFact()
	{
		ISelStrategyFact* fact = new ZtSelStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteSelStrategyFact(ISelStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
};


ZtSelStraFact::ZtSelStraFact()
{
}


ZtSelStraFact::~ZtSelStraFact()
{
}

SelStrategy* ZtSelStraFact::createStrategy(const char* name, const char* id)
{
	if (strcmp(name, "DualThrustSelection") == 0)
		return new ZtStraDtSel(id);

	return NULL;
}

bool ZtSelStraFact::deleteStrategy(SelStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}

void ZtSelStraFact::enumStrategy(FuncEnumSelStrategyCallback cb)
{
	cb(FACT_NAME, "DualThrustSelection", true);
}

const char* ZtSelStraFact::getName()
{
	return FACT_NAME;
}