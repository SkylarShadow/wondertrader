#pragma once
#include "../Includes/ZTSMarcos.h"

typedef void(PORTER_FLAG *FuncMQCallback)(ZTUInt32 id, const char* topic, const char* data, ZTUInt32 dataLen);
typedef void(PORTER_FLAG *FuncLogCallback)(ZTUInt32 id, const char* message, bool bServer);