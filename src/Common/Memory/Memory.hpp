YGAME_SERVER_BEGIN
class MemoryManager : public Singleton<MemoryManager>
{
public:
	MemoryManager() : m_splitSize(1024)
	{

	}

	template<typename T>
	void InitObjPool(uint32 initCount, uint32 incCount)
	{
		AutoThreadRWLock autoLock(&m_objRWLock);
		ObjectPool<T>::getInstance()->Init(initCount, incCount);
	}

    template<typename T>
    T * New()
	{
		m_objRWLock.Lock();
		ObjectPool<T> * pool = ObjectPool<T>::getInstance();
		ObjectDeleter * objectDeleter = pool;
		if (m_objDeleters.find(objectDeleter) == m_objDeleters.end())
		{
			m_objDeleters.insert(objectDeleter);
		}
		m_objRWLock.Unlock();

		auto * data = pool->New();
		return (T*)data->object;
    }

	bool IncRefCount(void * p, uint32 count)
	{
		void * data = (((char*)p) - sizeof(ObjectDataHead));
		ObjectDataHead * head = (ObjectDataHead*)data;
		ObjectDeleter * objectDeleter = head->objectDeleter;

		m_objRWLock.RLock();
		if (m_objDeleters.find(objectDeleter) == m_objDeleters.end())
		{
			m_objRWLock.Unlock();
			assert(false);
			return false;
		}
		m_objRWLock.Unlock();
		return objectDeleter->IncRefCount(data, count);
	}

	bool DecRefCount(void * p, uint32 count)
	{
		void * data = (((char*)p) - sizeof(ObjectDataHead));
		ObjectDataHead * head = (ObjectDataHead*)data;
		ObjectDeleter * objectDeleter = head->objectDeleter;

		m_objRWLock.RLock();
		if (m_objDeleters.find(objectDeleter) == m_objDeleters.end())
		{
			m_objRWLock.Unlock();
			assert(false);
			return false;
		}
		m_objRWLock.Unlock();
		return objectDeleter->DecRefCount(data, count);
	}
    
    void Delete(void * p)
	{
		DecRefCount(p, 1);
    }

	bool InitMemoryPool(uint32 size, uint32 initCount, uint32 incCount)
	{
		AutoThreadRWLock autoLock(&m_memoryRWLock);
		uint32 mapSize = GetMapSize(size);
		auto itor = m_memoryPoolsMap.find(mapSize);
		if (itor != m_memoryPoolsMap.end())
		{
			return false;
		}
		MemoryPool * pool = new MemoryPool();
		pool->Init(mapSize, initCount, incCount);
		m_memoryPoolsMap[mapSize] = pool;
		m_memoryPoolsSet.insert(pool);
		return true;
	}

	void * Malloc(uint32 size)
	{
		uint32 mapSize = GetMapSize(size);
		MemoryPool * pool = NULL;
		m_memoryRWLock.Lock();
		auto itor = m_memoryPoolsMap.find(mapSize);
		if (itor == m_memoryPoolsMap.end())
		{
			if (!InitMemoryPool(mapSize, 4, 4))
			{
				assert(false);
				return NULL;
			}
			itor = m_memoryPoolsMap.find(mapSize);
		}
		if (itor == m_memoryPoolsMap.end())
		{
			assert(false);
			return NULL;
		}
		pool = itor->second;
		m_memoryRWLock.Unlock();
		MemoryData * data = pool->Malloc();
		return data->memory;
	}

	void Free(void * p)
	{
		MemoryData * data = (MemoryData*)(((char*)p) - sizeof(MemoryDataHead));
		m_memoryRWLock.RLock();
		if (m_memoryPoolsSet.find(data->head.memoryPool) == m_memoryPoolsSet.end())
		{
			m_memoryRWLock.Unlock();
			assert(false);
			return;
		}
		m_memoryRWLock.Unlock();
		data->head.memoryPool->Free(data);
	}

private:
	uint32 GetMapSize(uint32 size)
	{
		uint32 mapSize = size;
		if (size < m_splitSize)
		{
			mapSize = 1;
			while (mapSize < size)
			{
				mapSize = (mapSize << 1);
			}
		}
		else
		{
			mapSize = (size + m_splitSize - 1) / m_splitSize * m_splitSize;
		}
		return mapSize;
	}
	YSet<ObjectDeleter *> m_objDeleters;
	RWLock m_objRWLock;
	YSet<MemoryPool*> m_memoryPoolsSet;
	YMap<uint32, MemoryPool*> m_memoryPoolsMap;
	RWLock m_memoryRWLock;
	uint32 m_splitSize;
};


#define gMemory (*MemoryManager::getInstance())

YGAME_SERVER_END
