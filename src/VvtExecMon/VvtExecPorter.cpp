#include "VvtExecPorter.h"
#include "VvtExecRunner.h"

#include "../VvtCore/VvtHelper.h"
#include "../VvTSTools/VvTSLogger.h"
#include "../Includes/VvTSVersion.h"

#ifdef _WIN32
#   ifdef _WIN64
    char PLATFORM_NAME[] = "X64";
#   else
    char PLATFORM_NAME[] = "X86";
#   endif
#else
    char PLATFORM_NAME[] = "UNIX";
#endif

VvtExecRunner& getRunner()
{
	static VvtExecRunner runner;
	return runner;
}

void init_exec(VvTString logCfg, bool isFile /*= true*/)
{
	static bool inited = false;

	if (inited)
		return;

	getRunner().init(logCfg);

	inited = true;
}

void config_exec(VvTString cfgfile, bool isFile /*= true*/)
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

VvTString get_version()
{
	static std::string _ver;
	if (_ver.empty())
	{
		_ver = PLATFORM_NAME;
		_ver += " ";
		_ver += VVT_VERSION;
		_ver += " Build@";
		_ver += __DATE__;
		_ver += " ";
		_ver += __TIME__;
	}
	return _ver.c_str();
}

void write_log(unsigned int level, VvTString message, VvTString catName)
{
	if (strlen(catName) > 0)
	{
		VvTSLogger::log_raw_by_cat(catName, (VvTSLogLevel)level, message);
	}
	else
	{
		VvTSLogger::log_raw((VvTSLogLevel)level, message);
	}
}

void set_position(VvTString stdCode, double targetPos)
{
	getRunner().setPosition(stdCode, targetPos);
}

void commit_positions()
{
	getRunner().commitPositions();
}