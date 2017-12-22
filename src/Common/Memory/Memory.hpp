YGAME_SERVER_BEGIN
class MemoryManager : public Singleton<MemoryManager>
{
public:
	template<typename T>
	void Init(uint32 init_count, uint32 inc_count)
	{
		AutoThreadLock autoLock(&m_threadLock);
		ObjectPool<T>::getInstance()->Init(init_count, inc_count);
	}

    template<typename T>
    T * New()
	{
		AutoThreadLock autoLock(&m_threadLock);
		ObjectPool<T> * pool = ObjectPool<T>::getInstance();
		ObjectDeleter * objectDeleter = pool;
		if (m_deleters.find(objectDeleter) == m_deleters.end())
		{
			m_deleters.insert(objectDeleter);
		}
		auto * data = pool->New();
		return (T*)data->object;
    }
    
    void Delete(void * p)
	{
		AutoThreadLock autoLock(&m_threadLock);
		void * data = (((char*)p) - sizeof(ObjectDeleter*));
		ObjectDeleter * objectDeleter = *(ObjectDeleter**)data;
		if (m_deleters.find(objectDeleter) == m_deleters.end())
		{
			assert(false);
			return;
		}
		objectDeleter->Delete(data);
    }

private:
	YSet<ObjectDeleter *> m_deleters;
	SpinLock m_threadLock;
};


#define gMemory (*MemoryManager::getInstance())

YGAME_SERVER_END
