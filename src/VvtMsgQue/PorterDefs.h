#pragma once
#include "../Includes/VvTSMarcos.h"

typedef void(PORTER_FLAG *FuncMQCallback)(VvTUInt32 id, const char* topic, const char* data, VvTUInt32 dataLen);
typedef void(PORTER_FLAG *FuncLogCallback)(VvTUInt32 id, const char* message, bool bServer);