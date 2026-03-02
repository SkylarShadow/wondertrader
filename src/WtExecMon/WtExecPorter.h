#pragma once
#include <stdint.h>
#include "../Includes/VVTSMarcos.h"

typedef const char*			VvtString;

#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG	void		init_exec(VvtString logCfg, bool isFile = true);

	EXPORT_FLAG	void		config_exec(VvtString cfgfile, bool isFile = true);

	EXPORT_FLAG	void		run_exec();

	EXPORT_FLAG	void		write_log(unsigned int level, VvtString message, VvtString catName);

	EXPORT_FLAG	VvtString	get_version();

	EXPORT_FLAG	void		release_exec();

	EXPORT_FLAG	void		set_position(VvtString stdCode, double targetPos);

	EXPORT_FLAG	void		commit_positions();

#ifdef __cplusplus
}
#endif