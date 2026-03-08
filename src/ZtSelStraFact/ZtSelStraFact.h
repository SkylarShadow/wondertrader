#pragma once
#include "../Includes/SelStrategyDefs.h"

USING_NS_ZTP;

class ZtSelStraFact : public ISelStrategyFact
{
public:
	ZtSelStraFact();
	virtual ~ZtSelStraFact();

public:
	virtual const char* getName() override;

	virtual SelStrategy* createStrategy(const char* name, const char* id) override;

	virtual void enumStrategy(FuncEnumSelStrategyCallback cb) override;

	virtual bool deleteStrategy(SelStrategy* stra) override;
};

