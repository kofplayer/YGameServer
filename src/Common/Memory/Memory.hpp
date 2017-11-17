﻿YGAME_SERVER_BEGIN
class MemoryManager : public Singleton<MemoryManager>
{
public:
	template<typename T>
	void Init(uint32 init_count, uint32 inc_count)
	{
		AutoThreadMutex autoMutex(&m_threadMutex);
		ObjectPool<T>::getInstance()->Init(init_count, inc_count);
	}

    template<typename T>
    T * New()
	{
		AutoThreadMutex autoMutex(&m_threadMutex);
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
		AutoThreadMutex autoMutex(&m_threadMutex);
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
	Set<ObjectDeleter *> m_deleters;
	ThreadMutex m_threadMutex;
};


#define gMemory (*MemoryManager::getInstance())

YGAME_SERVER_END