YGAME_SERVER_BEGIN
class MemoryManager : public Singleton<MemoryManager>
{
public:
	template<typename T>
	void Init(uint32 init_count, uint32 inc_count)
	{
		m_rwLock.Lock();
		ObjectPool<T>::getInstance()->Init(init_count, inc_count);
		m_rwLock.Unlock();
	}

    template<typename T>
    T * New()
	{
		m_rwLock.RLock();
		ObjectPool<T> * pool = ObjectPool<T>::getInstance();
		ObjectDeleter * objectDeleter = pool;
		if (m_deleters.find(objectDeleter) == m_deleters.end())
		{
			m_deleters.insert(objectDeleter);
		}
		m_rwLock.Unlock();

		auto * data = pool->New();
		return (T*)data->object;
    }

	bool IncRefCount(void * p, uint32 count)
	{
		void * data = (((char*)p) - sizeof(ObjectDataHead));
		ObjectDataHead * head = (ObjectDataHead*)data;
		ObjectDeleter * objectDeleter = head->objectDeleter;

		m_rwLock.RLock();
		if (m_deleters.find(objectDeleter) == m_deleters.end())
		{
			m_rwLock.Unlock();
			assert(false);
			return false;
		}
		m_rwLock.Unlock();
		return objectDeleter->IncRefCount(data, count);
	}

	bool DecRefCount(void * p, uint32 count)
	{
		void * data = (((char*)p) - sizeof(ObjectDataHead));
		ObjectDataHead * head = (ObjectDataHead*)data;
		ObjectDeleter * objectDeleter = head->objectDeleter;

		m_rwLock.RLock();
		if (m_deleters.find(objectDeleter) == m_deleters.end())
		{
			m_rwLock.Unlock();
			assert(false);
			return false;
		}
		m_rwLock.Unlock();
		return objectDeleter->DecRefCount(data, count);
	}
    
    void Delete(void * p)
	{
		DecRefCount(p, 1);
    }

	void * Malloc(uint32 size)
	{
		return malloc(size);
	}

	void Free(void * data)
	{
		free(data);
	}

private:
	YSet<ObjectDeleter *> m_deleters;
	RWLock m_rwLock;
};


#define gMemory (*MemoryManager::getInstance())

YGAME_SERVER_END
