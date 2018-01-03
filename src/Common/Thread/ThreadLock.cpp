#include "../Common.h"

YGAME_SERVER_BEGIN
MutexLock::MutexLock()
{
	THREAD_MUTEX_INIT(m_mutex);
}

MutexLock::~MutexLock()
{
	THREAD_MUTEX_DELETE(m_mutex);
}

void MutexLock::Lock()
{
	THREAD_MUTEX_LOCK(m_mutex);
}

void MutexLock::Unlock()
{
	THREAD_MUTEX_UNLOCK(m_mutex);
}

#if CURRENT_PLATFORM == PLATFORM_UNIX

SpinLock::SpinLock()
{
    pthread_spin_init(&m_spinLock, 0);
}
SpinLock::~SpinLock()
{
    pthread_spin_destroy( &m_spinLock );
}
void SpinLock::Lock()
{
    pthread_spin_lock( & m_spinLock );
}
void SpinLock::Unlock()
{
    pthread_spin_unlock( &m_spinLock );
}

#endif

#if CURRENT_PLATFORM != PLATFORM_WIN32
RWLock::RWLock()
{
    pthread_rwlock_init( &m_rwLock,  NULL );
}
RWLock::~RWLock()
{
    pthread_rwlock_destroy( &m_rwLock );
}
void RWLock::Lock()
{
    pthread_rwlock_wrlock( & m_rwLock );
}
void RWLock::RLock()
{
    pthread_rwlock_rdlock( & m_rwLock );
}
void RWLock::Unlock()
{
    pthread_rwlock_unlock( &m_rwLock );
}
#else
RWLock::RWLock()
{
	THREAD_MUTEX_INIT(m_mutex);
}
RWLock::~RWLock()
{
	THREAD_MUTEX_DELETE(m_mutex);
}
void RWLock::Lock()
{
	THREAD_MUTEX_LOCK(m_mutex);
}
void RWLock::RLock()
{
	THREAD_MUTEX_LOCK(m_mutex);
}
void RWLock::Unlock()
{
	THREAD_MUTEX_UNLOCK(m_mutex);
}
#endif

AutoThreadLock::AutoThreadLock(ThreadLock * threadLock) : m_threadLock(threadLock)
{
	m_threadLock->Lock();
}

AutoThreadLock::~AutoThreadLock()
{
	m_threadLock->Unlock();
}

AutoThreadRWLock::AutoThreadRWLock(ThreadRWLock * rwlock)
{
    m_rwLock = rwlock;
    m_rwLock->Lock();
}

AutoThreadRWLock::~AutoThreadRWLock()
{
    m_rwLock->Unlock();
}

AutoThreadRLock::AutoThreadRLock(ThreadRWLock * rwlock)
{
    m_rwLock = rwlock;
    m_rwLock->RLock();
}

AutoThreadRLock::~AutoThreadRLock()
{
    m_rwLock->Unlock();
}

YGAME_SERVER_END
