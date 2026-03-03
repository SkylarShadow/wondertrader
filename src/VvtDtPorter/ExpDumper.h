#pragma once
#include "../Includes/IDataWriter.h"

USING_NS_VVTP;

class ExpDumper : public IHisDataDumper
{
public:
	ExpDumper(const char* id) :_id(id) {}
	virtual ~ExpDumper() {}

public:
	virtual bool dumpHisBars(const char* stdCode, const char* period, VvTSBarStruct* bars, uint32_t count) override;

	virtual bool dumpHisTicks(const char* stdCode, uint32_t uDate, VvTSTickStruct* ticks, uint32_t count) override;

	virtual bool dumpHisOrdQue(const char* stdCode, uint32_t uDate, VvTSOrdQueStruct* items, uint32_t count) override;

	virtual bool dumpHisOrdDtl(const char* stdCode, uint32_t uDate, VvTSOrdDtlStruct* items, uint32_t count) override;

	virtual bool dumpHisTrans(const char* stdCode, uint32_t uDate, VvTSTransStruct* items, uint32_t count) override;

private:
	std::string	_id;
};

