#pragma once
#include <stdint.h>
#include "../Includes/ZTSMarcos.h"

typedef const char*			ZTString;

#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG	void		init_exec(ZTString logCfg, bool isFile = true);

	EXPORT_FLAG	void		config_exec(ZTString cfgfile, bool isFile = true);

	EXPORT_FLAG	void		run_exec();

	EXPORT_FLAG	void		write_log(unsigned int level, ZTString message, ZTString catName);

	EXPORT_FLAG	ZTString	get_version();

	EXPORT_FLAG	void		release_exec();

	EXPORT_FLAG	void		set_position(ZTString stdCode, double targetPos);

	EXPORT_FLAG	void		commit_positions();

#ifdef __cplusplus
}
#endif