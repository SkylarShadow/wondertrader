#pragma once
#include "../Includes/VVTSMarcos.h"

typedef void(PORTER_FLAG *FuncMQCallback)(VvtUInt32 id, const char* topic, const char* data, VvtUInt32 dataLen);
typedef void(PORTER_FLAG *FuncLogCallback)(VvtUInt32 id, const char* message, bool bServer);