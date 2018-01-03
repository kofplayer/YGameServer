YGAME_SERVER_BEGIN

class ThreadLock
{
public:
	ThreadLock() {}
	virtual ~ThreadLock() {}
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
};

class ThreadRWLock
{
public:
    ThreadRWLock() {}
    virtual ~ThreadRWLock() {}
    virtual void Lock() = 0;
    virtual void RLock() = 0;
    virtual void Unlock() = 0;
};

class ThreadNoneLock: public ThreadLock
{
public:
    virtual void Lock() {}
    virtual void Unlock() {}
};

class MutexLock : public ThreadLock
{
public:
	MutexLock();
	virtual ~MutexLock();
	virtual void Lock();
	virtual void Unlock();
protected:
	THREAD_MUTEX m_mutex;
};

#if CURRENT_PLATFORM == PLATFORM_UNIX

class SpinLock : public ThreadLock
{
public:
    SpinLock();
    virtual ~SpinLock();
    virtual void Lock();
    virtual void Unlock();
private:
    pthread_spinlock_t		m_spinLock;
};

#else
class SpinLock : public MutexLock {};

#endif

class RWLock : public ThreadRWLock
{
public:
    RWLock();
    virtual ~RWLock();
    virtual void Lock();
    virtual void RLock();
    virtual void Unlock();
private:
#if CURRENT_PLATFORM != PLATFORM_WIN32
	pthread_rwlock_t		m_rwLock;
#else
	THREAD_MUTEX			m_mutex;
#endif
};

class AutoThreadLock
{
public:
	AutoThreadLock(ThreadLock * threadLock);
	virtual ~AutoThreadLock();
protected:
	ThreadLock * m_threadLock;
};

class AutoThreadRWLock
{
public:
    AutoThreadRWLock(ThreadRWLock * rwlock);
    virtual ~AutoThreadRWLock();
private:
    ThreadRWLock * m_rwLock;
};

class AutoThreadRLock
{
public:
    AutoThreadRLock(ThreadRWLock * rwlock);
    virtual ~AutoThreadRLock();
private:
    ThreadRWLock * m_rwLock;
};

YGAME_SERVER_END
