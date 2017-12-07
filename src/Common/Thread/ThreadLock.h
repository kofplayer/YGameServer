YGAME_SERVER_BEGIN

class IThreadLock
{
public:
	IThreadLock() {}
	virtual ~IThreadLock() {}
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

class ThreadMutex : public IThreadLock
{
public:
	ThreadMutex();
	virtual ~ThreadMutex();
	virtual void lock();
	virtual void unlock();
protected:
	THREAD_MUTEX m_mutex;
};


class AutoThreadLock
{
public:
	AutoThreadLock(IThreadLock * lock);
	virtual ~AutoThreadLock();
protected:
	IThreadLock * m_threadLock;
};

YGAME_SERVER_END
