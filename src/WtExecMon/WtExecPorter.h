#pragma once
#include <stdint.h>
#include "../Includes/VvTSMarcos.h"

typedef const char*			VvTString;

#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG	void		init_exec(VvTString logCfg, bool isFile = true);

	EXPORT_FLAG	void		config_exec(VvTString cfgfile, bool isFile = true);

	EXPORT_FLAG	void		run_exec();

	EXPORT_FLAG	void		write_log(unsigned int level, VvTString message, VvTString catName);

	EXPORT_FLAG	VvTString	get_version();

	EXPORT_FLAG	void		release_exec();

	EXPORT_FLAG	void		set_position(VvTString stdCode, double targetPos);

	EXPORT_FLAG	void		commit_positions();

#ifdef __cplusplus
}
#endif