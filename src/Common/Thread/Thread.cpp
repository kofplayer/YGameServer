#include "../Common.h"

YGAME_SERVER_BEGIN
Thread::Thread()
{
}

Thread::~Thread()
{
    
}
void Thread::run()
{
}

void Thread::start()
{
#if CURRENT_PLATFORM == PLATFORM_WIN32
	tidp_ = _beginthreadex(NULL, 0, &Thread::threadFunc, (void*)this, 0, NULL);
#else	
	pthread_create(&tidp_, NULL, Thread::threadFunc, (void*)this);
#endif
}

#if CURRENT_PLATFORM == PLATFORM_WIN32
unsigned __stdcall Thread::threadFunc(void *arg)
#else	
void* Thread::threadFunc(void* arg)
#endif
{
	Thread * tptd = static_cast<Thread*>(arg);
	tptd->run();
#if CURRENT_PLATFORM == PLATFORM_WIN32
	return 0;
#else
	return NULL;
#endif		
}
YGAME_SERVER_END