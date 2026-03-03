/*!
 * \file VvtHelper.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "VvtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include <boost/filesystem.hpp>

#ifdef _MSC_VER
#include <direct.h>
#else	//UNIX
#include <unistd.h>
#endif

uint32_t VvtHelper::_cur_date = 0;
uint32_t VvtHelper::_cur_time = 0;
uint32_t VvtHelper::_cur_secs = 0;
uint32_t VvtHelper::_cur_tdate = 0;
std::string VvtHelper::_inst_dir;
std::string VvtHelper::_gen_dir = "./generated/";


std::string VvtHelper::getCWD()
{
	static std::string _cwd;
	if(_cwd.empty())
	{
		char   buffer[256];
#ifdef _MSC_VER
		_getcwd(buffer, 255);
#else	//UNIX
		getcwd(buffer, 255);
#endif
		_cwd = StrUtil::standardisePath(buffer);
	}	
	return _cwd;
}

std::string VvtHelper::getModulePath(const char* moduleName, const char* subDir, bool isCWD /* = true */)
{
	std::stringstream ss;
	ss << (isCWD?getCWD():getInstDir()) << subDir << "/" << moduleName;
	return ss.str();
}

const char* VvtHelper::getStraDataDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "stradata/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* VvtHelper::getStraUsrDatDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "userdata/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* VvtHelper::getPortifolioDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "portfolio/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* VvtHelper::getOutputDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "outputs/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* VvtHelper::getBaseDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir);
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}