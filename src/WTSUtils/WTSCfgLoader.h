#pragma once
#include "../Includes/VVTSMarcos.h"

#include <string>

NS_VVTP_BEGIN
class VVTSVariant;
NS_VVTP_END

USING_NS_VVTP;

class WTSCfgLoader
{
	static VVTSVariant*	load_from_json(const char* content);
	static VVTSVariant*	load_from_yaml(const char* content);

public:
	static VVTSVariant*	load_from_file(const char* filename);
	static VVTSVariant*	load_from_content(const std::string& content, bool isYaml = false);

	static VVTSVariant*	load_from_file(const std::string& filename)
	{
		return load_from_file(filename.c_str());
	}
};

