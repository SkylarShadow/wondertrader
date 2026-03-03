#include "VvtSelStraFact.h"
#include "VvtStraDtSel.h"

#include <string.h>
#include <boost/config.hpp>

const char* FACT_NAME = "VvtSelStraFact";


extern "C"
{
	EXPORT_FLAG ISelStrategyFact* createSelStrategyFact()
	{
		ISelStrategyFact* fact = new VvtSelStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteSelStrategyFact(ISelStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
};


VvtSelStraFact::VvtSelStraFact()
{
}


VvtSelStraFact::~VvtSelStraFact()
{
}

SelStrategy* VvtSelStraFact::createStrategy(const char* name, const char* id)
{
	if (strcmp(name, "DualThrustSelection") == 0)
		return new VvtStraDtSel(id);

	return NULL;
}

bool VvtSelStraFact::deleteStrategy(SelStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}

void VvtSelStraFact::enumStrategy(FuncEnumSelStrategyCallback cb)
{
	cb(FACT_NAME, "DualThrustSelection", true);
}

const char* VvtSelStraFact::getName()
{
	return FACT_NAME;
}