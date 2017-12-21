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
#include <stdio.h>

// current platform and compiler
#define PLATFORM_WIN32			0
#define PLATFORM_UNIX			1
#define PLATFORM_APPLE			2

#define UNIX_FLAVOUR_LINUX		1
#define UNIX_FLAVOUR_BSD		2
#define UNIX_FLAVOUR_OTHER		3
#define UNIX_FLAVOUR_OSX		4

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#define CURRENT_PLATFORM PLATFORM_WIN32
#elif defined( __INTEL_COMPILER )
#define CURRENT_PLATFORM PLATFORM_INTEL
#elif defined( __APPLE_CC__ )
#define CURRENT_PLATFORM PLATFORM_APPLE
#else
#define CURRENT_PLATFORM PLATFORM_UNIX
#endif

#if CURRENT_PLATFORM == PLATFORM_WIN32
#include <winsock2.h>		// 必须在windows.h之前包含， 否则网络模块编译会出错
#include "../../dependencies/mysql/include/windows/mysql.h"
#pragma comment(lib, "../../dependencies/mysql/lib/windows/libmysql.lib")
#include <WS2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#include <process.h>
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include "mysql/include/linux/mysql.h"
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <netinet/ip.h>
#include <stdarg.h>
#endif

#if CURRENT_PLATFORM == PLATFORM_UNIX
#define HAS_EPOLL
#endif

#ifdef HAS_EPOLL
#include <sys/epoll.h>
#endif

#include "./CommonDefine/CommonDefine.h"
#include "./Singleton/Singleton.hpp"
#include "./Thread/ThreadLock.h"
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
#include "./DataBase/DBResult.h"
#include "./DataBase/DataBase.h"
#include "./DataBase/DBResultMySql.h"
#include "./DataBase/DataBaseMySql.h"

#endif
