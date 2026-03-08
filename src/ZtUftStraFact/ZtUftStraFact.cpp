#include "ZtUftStraFact.h"
#include "ZtUftStraDemo.h"

#include <string.h>

const char* FACT_NAME = "ZtUftStraFact";

extern "C"
{
	EXPORT_FLAG IUftStrategyFact* createStrategyFact()
	{
		IUftStrategyFact* fact = new ZtUftStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteStrategyFact(IUftStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


ZtUftStraFact::ZtUftStraFact()
{
}


ZtUftStraFact::~ZtUftStraFact()
{
}

const char* ZtUftStraFact::getName()
{
	return FACT_NAME;
}

void ZtUftStraFact::enumStrategy(FuncEnumUftStrategyCallback cb)
{
	cb(FACT_NAME, "SimpleUft", true);
}

UftStrategy* ZtUftStraFact::createStrategy(const char* name, const char* id)
{
	if(strcmp(name, "SimpleUft") == 0)
	{
		return new ZtUftStraDemo(id);
	}


	return NULL;
}

bool ZtUftStraFact::deleteStrategy(UftStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}
