#pragma once
#include "../Includes/VvTSMarcos.h"

#include <string>

NS_VVTP_BEGIN
class VvTSVariant;
NS_VVTP_END

USING_NS_VVTP;

class WTSCfgLoader
{
	static VvTSVariant*	load_from_json(const char* content);
	static VvTSVariant*	load_from_yaml(const char* content);

public:
	static VvTSVariant*	load_from_file(const char* filename);
	static VvTSVariant*	load_from_content(const std::string& content, bool isYaml = false);

	static VvTSVariant*	load_from_file(const std::string& filename)
	{
		return load_from_file(filename.c_str());
	}
};

