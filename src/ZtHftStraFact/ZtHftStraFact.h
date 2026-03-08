#pragma once
#include "../Includes/HftStrategyDefs.h"

USING_NS_ZTP;

class ZtHftStraFact : public IHftStrategyFact
{
public:
	ZtHftStraFact();
	virtual ~ZtHftStraFact();

public:
	virtual const char* getName() override;

	virtual void enumStrategy(FuncEnumHftStrategyCallback cb) override;

	virtual HftStrategy* createStrategy(const char* name, const char* id) override;

	virtual bool deleteStrategy(HftStrategy* stra) override;
};

