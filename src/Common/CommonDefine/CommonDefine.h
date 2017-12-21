YGAME_SERVER_BEGIN

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
inline const char* getStrError(int32 ierrorno = 0)
{
#if CURRENT_PLATFORM == PLATFORM_WIN32
	if (ierrorno == 0)
		ierrorno = GetLastError();

	static char lpMsgBuf[256] = { 0 };
	snprintf(lpMsgBuf, 256, "errorno=%d", ierrorno);
	return lpMsgBuf;
#elif CURRENT_PLATFORM == PLATFORM_APPLE
    static const char * sError = "error";
    return sError;
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
#elif CURRENT_PLATFORM == PLATFORM_APPLE
    return 1;
#else
	return errno;
#endif
}

YGAME_SERVER_END
