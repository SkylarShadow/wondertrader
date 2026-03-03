#include "VvtHftStraFact.h"
#include "VvtHftStraDemo.h"

#include <string.h>

const char* FACT_NAME = "VvtHftStraFact";

extern "C"
{
	EXPORT_FLAG IHftStrategyFact* createStrategyFact()
	{
		IHftStrategyFact* fact = new VvtHftStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteStrategyFact(IHftStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


VvtHftStraFact::VvtHftStraFact()
{
}


VvtHftStraFact::~VvtHftStraFact()
{
}

const char* VvtHftStraFact::getName()
{
	return FACT_NAME;
}

void VvtHftStraFact::enumStrategy(FuncEnumHftStrategyCallback cb)
{
	cb(FACT_NAME, "SimpleHft", true);
}

HftStrategy* VvtHftStraFact::createStrategy(const char* name, const char* id)
{
	if(strcmp(name, "SimpleHft") == 0)
	{
		return new VvtHftStraDemo(id);
	}


	return NULL;
}

bool VvtHftStraFact::deleteStrategy(HftStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}
