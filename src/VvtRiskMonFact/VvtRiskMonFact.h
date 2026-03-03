/*!
 * \file VvtRiskMonFact.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#pragma once
#include "../Includes/RiskMonDefs.h"

USING_NS_VVTP;

class VvtRiskMonFact : public IRiskMonitorFact
{
public:
	VvtRiskMonFact();
	virtual ~VvtRiskMonFact();

public:
	virtual const char* getName() override;
	virtual void enumRiskMonitors(FuncEnumRiskMonCallback cb) override;

	virtual VvtRiskMonitor* createRiskMonotor(const char* name) override;

	virtual bool deleteRiskMonotor(VvtRiskMonitor* unit) override;

};

