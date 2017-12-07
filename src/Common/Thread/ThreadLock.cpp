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

void MutexLock::lock()
{
	THREAD_MUTEX_LOCK(m_mutex);
}

void MutexLock::unlock()
{
	THREAD_MUTEX_UNLOCK(m_mutex);
}

#if CURRENT_PLATFORM == PLATFORM_UNIX

SpinLock::SpinLock()
{
    pthread_spin_init(&m_spinlock, 0);
}
SpinLock::~SpinLock()
{
    pthread_spin_destroy( &m_spinlock );
}
void SpinLock::lock()
{
    pthread_spin_lock( & m_spinlock );
}
void SpinLock::unlock()
{
    pthread_spin_unlock( &m_spinlock );
}

#endif

RWLock::RWLock()
{
    pthread_rwlock_init( &m_rwlock,  NULL );
}
RWLock::~RWLock()
{
    pthread_rwlock_destroy( &m_rwlock );
}
void RWLock::lock()
{
    pthread_rwlock_wrlock( & m_rwlock );
}
void RWLock::rlock()
{
    pthread_rwlock_rdlock( & m_rwlock );
}
void RWLock::unlock()
{
    pthread_rwlock_unlock( &m_rwlock );
}


AutoThreadLock::AutoThreadLock(ThreadLock * threadLock) : m_threadLock(threadLock)
{
	m_threadLock->lock();
}

AutoThreadLock::~AutoThreadLock()
{
	m_threadLock->unlock();
}

AutoThreadRWLock::AutoThreadRWLock(ThreadRWLock * rwlock)
{
    m_rwlock = rwlock;
    m_rwlock->lock();
}

AutoThreadRWLock::~AutoThreadRWLock()
{
    m_rwlock->unlock();
}

AutoThreadRLock::AutoThreadRLock(ThreadRWLock * rwlock)
{
    m_rwlock = rwlock;
    m_rwlock->rlock();
}

AutoThreadRLock::~AutoThreadRLock()
{
    m_rwlock->unlock();
}

YGAME_SERVER_END
