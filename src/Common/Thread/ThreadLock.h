YGAME_SERVER_BEGIN

class ThreadLock
{
public:
	ThreadLock() {}
	virtual ~ThreadLock() {}
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

class ThreadRWLock
{
public:
    ThreadRWLock() {}
    virtual ~ThreadRWLock() {}
    virtual void lock() = 0;
    virtual void rlock() = 0;
    virtual void unlock() = 0;
};

class ThreadNoneLock: public ThreadLock
{
public:
    virtual void lock() {}
    virtual void unlock() {}
};

class MutexLock : public ThreadLock
{
public:
	MutexLock();
	virtual ~MutexLock();
	virtual void lock();
	virtual void unlock();
protected:
	THREAD_MUTEX m_mutex;
};

#if CURRENT_PLATFORM == PLATFORM_UNIX

class SpinLock : public ThreadLock
{
public:
    SpinLock();
    virtual ~SpinLock();
    virtual void lock();
    virtual void unlock();
private:
    pthread_spinlock_t		m_spinlock;
};

#else
class SpinLock : public MutexLock {};

#endif

class RWLock : public ThreadRWLock
{
public:
    RWLock();
    virtual ~RWLock();
    virtual void lock();
    virtual void rlock();
    virtual void unlock();
private:
    pthread_rwlock_t		m_rwlock;
};

class AutoThreadLock
{
public:
	AutoThreadLock(ThreadLock * lock);
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
    ThreadRWLock * m_rwlock;
};

class AutoThreadRLock
{
public:
    AutoThreadRLock(ThreadRWLock * rwlock);
    virtual ~AutoThreadRLock();
private:
    ThreadRWLock * m_rwlock;
};

YGAME_SERVER_END
