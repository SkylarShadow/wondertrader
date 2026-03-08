#include "ZtExecuterFactory.h"

#include "../Share/StdUtils.hpp"
#include "../Share/StrUtil.hpp"
#include "../ZTSTools/ZTSLogger.h"

#include <boost/filesystem.hpp>


USING_NS_ZTP;

//////////////////////////////////////////////////////////////////////////
//ZtExecuterFactory
bool ZtExecuterFactory::loadFactories(const char* path)
{
	if (!StdFile::exists(path))
	{
		ZTSLogger::error("Directory {} of executer factory not exists", path);
		return false;
	}

	boost::filesystem::path myPath(path);
	boost::filesystem::directory_iterator endIter;
	for (boost::filesystem::directory_iterator iter(myPath); iter != endIter; iter++)
	{
		if (boost::filesystem::is_directory(iter->path()))
			continue;

#ifdef _WIN32
		if (iter->path().extension() != ".dll")
			continue;
#else //_UNIX
		if (iter->path().extension() != ".so")
			continue;
#endif

		const std::string& path = iter->path().string();

		DllHandle hInst = DLLHelper::load_library(path.c_str());
		if (hInst == NULL)
		{
			continue;
		}

		FuncCreateExeFact creator = (FuncCreateExeFact)DLLHelper::get_symbol(hInst, "createExecFact");
		if (creator == NULL)
		{
			DLLHelper::free_library(hInst);
			continue;
		}

		ExeFactInfo fInfo;
		fInfo._module_inst = hInst;
		fInfo._module_path = iter->path().string();
		fInfo._creator = creator;
		fInfo._remover = (FuncDeleteExeFact)DLLHelper::get_symbol(hInst, "deleteExecFact");
		fInfo._fact = fInfo._creator();

		_factories[fInfo._fact->getName()] = fInfo;

		ZTSLogger::info("Executer factory {} loaded", fInfo._fact->getName());
	}

	return true;
}

ExecuteUnitPtr ZtExecuterFactory::createExeUnit(const char* factname, const char* unitname)
{
	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = (ExeFactInfo&)it->second;
	ExecuteUnit* unit = fInfo._fact->createExeUnit(unitname);
	if (unit == NULL)
	{
		ZTSLogger::error("Createing execution unit failed: {}.{}", factname, unitname);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}

ExecuteUnitPtr ZtExecuterFactory::createDiffExeUnit(const char* factname, const char* unitname)
{
	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = (ExeFactInfo&)it->second;
	ExecuteUnit* unit = fInfo._fact->createDiffExeUnit(unitname);
	if (unit == NULL)
	{
		ZTSLogger::error("Createing diff execution unit failed: {}.{}", factname, unitname);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}

ExecuteUnitPtr ZtExecuterFactory::createArbiExeUnit(const char* factname, const char* unitname)
{
	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = (ExeFactInfo&)it->second;
	ExecuteUnit* unit = fInfo._fact->createArbiExeUnit(unitname);
	if (unit == NULL)
	{
		ZTSLogger::error("Createing arbi execution unit failed: {}.{}", factname, unitname);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}

ExecuteUnitPtr ZtExecuterFactory::createExeUnit(const char* name)
{
	StringVector ay = StrUtil::split(name, ".");
	if (ay.size() < 2)
		return ExecuteUnitPtr();

	const char* factname = ay[0].c_str();
	const char* unitname = ay[1].c_str();

	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = (ExeFactInfo&)it->second;
	ExecuteUnit* unit = fInfo._fact->createExeUnit(unitname);
	if (unit == NULL)
	{
		ZTSLogger::error("Createing execution unit failed: {}", name);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}

ExecuteUnitPtr ZtExecuterFactory::createDiffExeUnit(const char* name)
{
	StringVector ay = StrUtil::split(name, ".");
	if (ay.size() < 2)
		return ExecuteUnitPtr();

	const char* factname = ay[0].c_str();
	const char* unitname = ay[1].c_str();

	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = (ExeFactInfo&)it->second;
	ExecuteUnit* unit = fInfo._fact->createDiffExeUnit(unitname);
	if (unit == NULL)
	{
		ZTSLogger::error("Createing execution unit failed: {}", name);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}

ExecuteUnitPtr ZtExecuterFactory::createArbiExeUnit(const char* name)
{
	StringVector ay = StrUtil::split(name, ".");
	if (ay.size() < 2)
		return ExecuteUnitPtr();

	const char* factname = ay[0].c_str();
	const char* unitname = ay[1].c_str();

	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = (ExeFactInfo&)it->second;
	ExecuteUnit* unit = fInfo._fact->createArbiExeUnit(unitname);
	if (unit == NULL)
	{
		ZTSLogger::error("Createing execution unit failed: {}", name);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}
