#include "ZtExecPorter.h"
#include "ZtExecRunner.h"

#include "../ZtCore/ZtHelper.h"
#include "../ZTSTools/ZTSLogger.h"
#include "../Includes/ZTSVersion.h"

#ifdef _WIN32
#   ifdef _WIN64
    char PLATFORM_NAME[] = "X64";
#   else
    char PLATFORM_NAME[] = "X86";
#   endif
#else
    char PLATFORM_NAME[] = "UNIX";
#endif

ZtExecRunner& getRunner()
{
	static ZtExecRunner runner;
	return runner;
}

void init_exec(ZTString logCfg, bool isFile /*= true*/)
{
	static bool inited = false;

	if (inited)
		return;

	getRunner().init(logCfg);

	inited = true;
}

void config_exec(ZTString cfgfile, bool isFile /*= true*/)
{
	if (strlen(cfgfile) == 0)
		getRunner().config("cfgexec.json");
	else
		getRunner().config(cfgfile);
}

void run_exec()
{
	getRunner().run();
}

void release_exec()
{
	getRunner().release();
}

ZTString get_version()
{
	static std::string _ver;
	if (_ver.empty())
	{
		_ver = PLATFORM_NAME;
		_ver += " ";
		_ver += ZT_VERSION;
		_ver += " Build@";
		_ver += __DATE__;
		_ver += " ";
		_ver += __TIME__;
	}
	return _ver.c_str();
}

void write_log(unsigned int level, ZTString message, ZTString catName)
{
	if (strlen(catName) > 0)
	{
		ZTSLogger::log_raw_by_cat(catName, (ZTSLogLevel)level, message);
	}
	else
	{
		ZTSLogger::log_raw((ZTSLogLevel)level, message);
	}
}

void set_position(ZTString stdCode, double targetPos)
{
	getRunner().setPosition(stdCode, targetPos);
}

void commit_positions()
{
	getRunner().commitPositions();
}