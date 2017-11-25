#ifndef YGAME_SERVER_COMMON
#define YGAME_SERVER_COMMON




#define YGAME_SERVER_NAMESPACE YGameServer
#define YGAME_SERVER_BEGIN namespace YGAME_SERVER_NAMESPACE {
#define YGAME_SERVER_END }

#include <stdlib.h>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <string.h>
#include <set>
#include <assert.h>
#include <time.h>

#include "./CommonDefine/CommonDefine.h"
#include "./Singleton/Singleton.hpp"
#include "./Thread/ThreadMutex.h"
#include "./Log/LogWriter.h"
#include "./Log/Log.h"
#include "./Log/FileLogWriter.h"
#include "./Log/ScreenLogWriter.h"
#include "./Memory/ObjectPool.hpp"
#include "./Coroutine/Protothread.h"
#include "./Coroutine/Coroutine.h"
#include "./Coroutine/CoroutineGroup.h"
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
#include "./Network/NetPoller.h"
#include "./Network/NetSelectPoller.h"
#include "./Network/NetEpollPoller.h"

#endif
