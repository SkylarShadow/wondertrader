#pragma once
#include "../Includes/HftStrategyDefs.h"

USING_NS_VVTP;

class VvtHftStraFact : public IHftStrategyFact
{
public:
	VvtHftStraFact();
	virtual ~VvtHftStraFact();

public:
	virtual const char* getName() override;

	virtual void enumStrategy(FuncEnumHftStrategyCallback cb) override;

	virtual HftStrategy* createStrategy(const char* name, const char* id) override;

	virtual bool deleteStrategy(HftStrategy* stra) override;
};

