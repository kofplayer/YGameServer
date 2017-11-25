YGAME_SERVER_BEGIN
#include <stdio.h>
/*
#include <stdlib.h>
#include <stdarg.h> 
#include <math.h>
#include <assert.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>  
#include <cstring>  
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <limits>
#include <algorithm>
#include <utility>
#include <functional>
#include <cctype>
#include <iterator>
#include "common/strutil.h"
*/

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
#include <WS2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#include <process.h>
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
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

/*
#if CURRENT_PLATFORM == PLATFORM_WIN32
#pragma warning(disable:4996)
#pragma warning(disable:4819)
#pragma warning(disable:4049)
#pragma warning(disable:4217)
#include <io.h>
#include <time.h> 
//#define FD_SETSIZE 1024
#ifndef WIN32_LEAN_AND_MEAN 
#include <winsock2.h>		// 必须在windows.h之前包含， 否则网络模块编译会出错
#include <mswsock.h> 
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <unordered_map>
#include <functional>
#include <memory>
#define _SCL_SECURE_NO_WARNINGS
#else
#include <errno.h>
#include <float.h>
#include <pthread.h>	
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <iconv.h>
#include <langinfo.h>
#include <stdint.h>
#include <signal.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <tr1/unordered_map>
#include <tr1/functional>
#include <tr1/memory>
#include <linux/types.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/resource.h> 
#include <linux/errqueue.h>
#endif
*/

/*---------------------------------------------------------------------------------
	定会多种平台上的多线程相关
---------------------------------------------------------------------------------*/
#if CURRENT_PLATFORM == PLATFORM_WIN32
	#define THREAD_ID											uintptr_t
	#define THREAD_SINGNAL										HANDLE
	#define THREAD_SINGNAL_INIT(x)								x = CreateEvent(NULL, TRUE, FALSE, NULL)
	#define THREAD_SINGNAL_DELETE(x)							CloseHandle(x)
	#define THREAD_SINGNAL_SET(x)								SetEvent(x)
	#define THREAD_MUTEX										CRITICAL_SECTION
	#define THREAD_MUTEX_INIT(x)								InitializeCriticalSection(&x)
	#define THREAD_MUTEX_DELETE(x)								DeleteCriticalSection(&x)
	#define THREAD_MUTEX_LOCK(x)								EnterCriticalSection(&x)
	#define THREAD_MUTEX_UNLOCK(x)								LeaveCriticalSection(&x)
	typedef int													SOCKET_ID;
	#define snprintf											_snprintf_s
#else
	#define THREAD_ID											pthread_t
	#define THREAD_SINGNAL										pthread_cond_t
	#define THREAD_SINGNAL_INIT(x)								pthread_cond_init(&x, NULL)
	#define THREAD_SINGNAL_DELETE(x)							pthread_cond_destroy(&x)
	#define THREAD_SINGNAL_SET(x)								pthread_cond_signal(&x);
	#define THREAD_MUTEX										pthread_mutex_t
	#define THREAD_MUTEX_INIT(x)								pthread_mutex_init (&x, NULL)
	#define THREAD_MUTEX_DELETE(x)								pthread_mutex_destroy(&x)
	#define THREAD_MUTEX_LOCK(x)								pthread_mutex_lock(&x)
	#define THREAD_MUTEX_UNLOCK(x)								pthread_mutex_unlock(&x)	
	typedef int													SOCKET_ID;
	#define INVALID_SOCKET										-1
#endif

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef uint32 NET_ADDR;
typedef uint16 NET_PORT;

#define Vector std::vector
#define Map std::map
#define List std::list
#define Set std::set
#define String std::string

#if CURRENT_PLATFORM == PLATFORM_WIN32
#else
#define Sleep(x) sleep(x/1000)
#endif

#define countof(array) (sizeof(array)/sizeof(array[0]))

// 获得系统产生的最后一次错误描述
inline char* getStrError(int32 ierrorno = 0)
{
#if CURRENT_PLATFORM == PLATFORM_WIN32
	if (ierrorno == 0)
		ierrorno = GetLastError();

	static char lpMsgBuf[256] = { 0 };
	snprintf(lpMsgBuf, 256, "errorno=%d", ierrorno);
	return lpMsgBuf;
#else
	if (ierrorno != 0)
		return strerror(ierrorno);
	return strerror(errno);
#endif
}

inline int getLastError()
{
#if CURRENT_PLATFORM == PLATFORM_WIN32
	return GetLastError();
#else
	return errno;
#endif
}

YGAME_SERVER_END
