/*!
 * \file ZtRiskMonFact.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#pragma once
#include "../Includes/RiskMonDefs.h"

USING_NS_ZTP;

class ZtRiskMonFact : public IRiskMonitorFact
{
public:
	ZtRiskMonFact();
	virtual ~ZtRiskMonFact();

public:
	virtual const char* getName() override;
	virtual void enumRiskMonitors(FuncEnumRiskMonCallback cb) override;

	virtual ZtRiskMonitor* createRiskMonotor(const char* name) override;

	virtual bool deleteRiskMonotor(ZtRiskMonitor* unit) override;

};

