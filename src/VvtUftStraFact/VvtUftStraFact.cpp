#include "VvtUftStraFact.h"
#include "VvtUftStraDemo.h"

#include <string.h>

const char* FACT_NAME = "VvtUftStraFact";

extern "C"
{
	EXPORT_FLAG IUftStrategyFact* createStrategyFact()
	{
		IUftStrategyFact* fact = new VvtUftStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteStrategyFact(IUftStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


VvtUftStraFact::VvtUftStraFact()
{
}


VvtUftStraFact::~VvtUftStraFact()
{
}

const char* VvtUftStraFact::getName()
{
	return FACT_NAME;
}

void VvtUftStraFact::enumStrategy(FuncEnumUftStrategyCallback cb)
{
	cb(FACT_NAME, "SimpleUft", true);
}

UftStrategy* VvtUftStraFact::createStrategy(const char* name, const char* id)
{
	if(strcmp(name, "SimpleUft") == 0)
	{
		return new VvtUftStraDemo(id);
	}


	return NULL;
}

bool VvtUftStraFact::deleteStrategy(UftStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}
