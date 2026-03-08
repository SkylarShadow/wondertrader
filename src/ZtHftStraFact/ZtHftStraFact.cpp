#include "ZtHftStraFact.h"
#include "ZtHftStraDemo.h"

#include <string.h>

const char* FACT_NAME = "ZtHftStraFact";

extern "C"
{
	EXPORT_FLAG IHftStrategyFact* createStrategyFact()
	{
		IHftStrategyFact* fact = new ZtHftStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteStrategyFact(IHftStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


ZtHftStraFact::ZtHftStraFact()
{
}


ZtHftStraFact::~ZtHftStraFact()
{
}

const char* ZtHftStraFact::getName()
{
	return FACT_NAME;
}

void ZtHftStraFact::enumStrategy(FuncEnumHftStrategyCallback cb)
{
	cb(FACT_NAME, "SimpleHft", true);
}

HftStrategy* ZtHftStraFact::createStrategy(const char* name, const char* id)
{
	if(strcmp(name, "SimpleHft") == 0)
	{
		return new ZtHftStraDemo(id);
	}


	return NULL;
}

bool ZtHftStraFact::deleteStrategy(HftStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}
