#ifndef YGAME_SERVER_COMMON
#define YGAME_SERVER_COMMON




#define YGAME_SERVER_NAMESPACE YGameServer
#define YGAME_SERVER_BEGIN namespace YGAME_SERVER_NAMESPACE {
#define YGAME_SERVER_END }

#include <stdlib.h>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <assert.h>

#include "./CommonDefine/CommonDefine.h"
#include "./Singleton/Singleton.hpp"
#include "./Memory/ObjectPool.hpp"
#include "./Thread/ThreadMutex.h"
#include "./Memory/Memory.hpp"
#include "./Event/EventFilter.hpp"
#include "./Event/EventHandler.h"
#include "./Event/EventMonitorNode.hpp"
#include "./Event/EventManager.h"
#include "./Task/Task.h"
#include "./Thread/Thread.h"
#include "./Network/NetBase.h"
#include "./Network/NetConnect.h"
#include "./Network/NetPConnect.h"
#include "./Network/NetAConnect.h"
#include "./Network/NetListener.h"





#endif
