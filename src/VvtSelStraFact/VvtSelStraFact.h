#pragma once
#include "../Includes/SelStrategyDefs.h"

USING_NS_VVTP;

class VvtSelStraFact : public ISelStrategyFact
{
public:
	VvtSelStraFact();
	virtual ~VvtSelStraFact();

public:
	virtual const char* getName() override;

	virtual SelStrategy* createStrategy(const char* name, const char* id) override;

	virtual void enumStrategy(FuncEnumSelStrategyCallback cb) override;

	virtual bool deleteStrategy(SelStrategy* stra) override;
};

