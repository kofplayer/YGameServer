YGAME_SERVER_BEGIN

class IThreadMutex
{
public:
	IThreadMutex() {}
	virtual ~IThreadMutex() {}
	virtual void lockMutex() = 0;
	virtual void unlockMutex() = 0;
};

class ThreadMutex : public IThreadMutex
{
public:
	ThreadMutex();
	virtual ~ThreadMutex();
	virtual void lockMutex();
	virtual void unlockMutex();
protected:
	THREAD_MUTEX m_mutex;
};


class AutoThreadMutex
{
public:
	AutoThreadMutex(IThreadMutex * mutex);
	virtual ~AutoThreadMutex();
protected:
	IThreadMutex * m_threadMutex;
};
YGAME_SERVER_END