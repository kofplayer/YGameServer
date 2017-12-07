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

void ThreadMutex::lock()
{
	THREAD_MUTEX_LOCK(m_mutex);
}

void ThreadMutex::unlock()
{
	THREAD_MUTEX_UNLOCK(m_mutex);
}

AutoThreadLock::AutoThreadLock(IThreadLock * threadLock) : m_threadLock(threadLock)
{
	m_threadLock->lock();
}

AutoThreadLock::~AutoThreadLock()
{
	m_threadLock->unlock();
}

YGAME_SERVER_END
