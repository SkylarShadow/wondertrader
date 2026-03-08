#pragma once
#include "../Includes/UftStrategyDefs.h"

USING_NS_ZTP;

class ZtUftStraFact : public IUftStrategyFact
{
public:
	ZtUftStraFact();
	virtual ~ZtUftStraFact();

public:
	virtual const char* getName() override;

	virtual void enumStrategy(FuncEnumUftStrategyCallback cb) override;

	virtual UftStrategy* createStrategy(const char* name, const char* id) override;

	virtual bool deleteStrategy(UftStrategy* stra) override;
};

