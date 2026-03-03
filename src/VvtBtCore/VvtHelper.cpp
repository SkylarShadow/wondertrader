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
#include <boost/filesystem.hpp>

#ifdef _MSC_VER
#include <direct.h>
#else	//UNIX
#include <unistd.h>
#endif

std::string VvtHelper::_inst_dir;
std::string VvtHelper::_out_dir = "./outputs_bt/";

std::string VvtHelper::getCWD()
{
	static std::string _cwd;
	if(_cwd.empty())
	{
		char   buffer[255];
#ifdef _MSC_VER
		_getcwd(buffer, 255);
#else	//UNIX
		getcwd(buffer, 255);
#endif
		_cwd = buffer;
		_cwd = StrUtil::standardisePath(_cwd);
	}	
	return _cwd;
}

void VvtHelper::setOutputDir(const char* out_dir)
{
	_out_dir = StrUtil::standardisePath(std::string(out_dir));
}

const char* VvtHelper::getOutputDir()
{
	if (!boost::filesystem::exists(_out_dir.c_str()))
        boost::filesystem::create_directories(_out_dir.c_str());
	return _out_dir.c_str();
}