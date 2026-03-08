#pragma once
#include "../Includes/ZTSMarcos.h"

#include <string>

NS_ZTP_BEGIN
class ZTSVariant;
NS_ZTP_END

USING_NS_ZTP;

class ZTSCfgLoader
{
	static ZTSVariant*	load_from_json(const char* content);
	static ZTSVariant*	load_from_yaml(const char* content);

public:
	static ZTSVariant*	load_from_file(const char* filename);
	static ZTSVariant*	load_from_content(const std::string& content, bool isYaml = false);

	static ZTSVariant*	load_from_file(const std::string& filename)
	{
		return load_from_file(filename.c_str());
	}
};

