#include "../Common.h"

YGAME_SERVER_BEGIN
ThreadMutex::ThreadMutex()
{
	THREAD_MUTEX_INIT(m_mutex);
}

ThreadMutex::~ThreadMutex()
{
	THREAD_MUTEX_DELETE(m_mutex);
}

void ThreadMutex::lockMutex()
{
	THREAD_MUTEX_LOCK(m_mutex);
}

void ThreadMutex::unlockMutex()
{
	THREAD_MUTEX_UNLOCK(m_mutex);
}

AutoThreadMutex::AutoThreadMutex(IThreadMutex * threadMutex) : m_threadMutex(threadMutex)
{
	m_threadMutex->lockMutex();
}

AutoThreadMutex::~AutoThreadMutex()
{
	m_threadMutex->unlockMutex();
}

YGAME_SERVER_END