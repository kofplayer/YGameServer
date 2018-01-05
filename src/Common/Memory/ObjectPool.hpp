YGAME_SERVER_BEGIN

class ObjectDeleter
{
public:
	ObjectDeleter() {}
	virtual ~ObjectDeleter() {}
	virtual bool IncRefCount(void * data, uint32 count) = 0;
	virtual bool DecRefCount(void * data, uint32 count) = 0;
};

#pragma pack(1)
struct ObjectDataHead
{
	uint32 refCount;
	ObjectDeleter * objectDeleter;
};
#pragma pack()

template<typename T>
class ObjectPool : public ObjectDeleter, public Singleton<ObjectPool<T>/**/>
{
	friend class MemoryManager;
	friend class Singleton<ObjectPool<T>/**/>;
protected:
#pragma pack(1)
	struct ObjectData
	{
		ObjectDataHead head;
		char object[sizeof(T)];
	};
#pragma pack()
	ObjectPool()
	{
		m_incCount = 16;
	}
	void Init(uint32 initCount, uint32 incCount)
	{
		AutoThreadLock autoLock(&m_threadLock);
		if (incCount != 0)
		{
			m_incCount = incCount;
		}
		if (m_used_objects.size() == 0 && m_free_objects.size() == 0)
		{
			assignObjs(initCount);
		}
	}

	ObjectData * New()
    {
		m_threadLock.Lock();
		if (m_free_objects.size()==0)
		{
			assignObjs(m_incCount);
		}
		auto itor = m_free_objects.begin();
		ObjectData* data = (*itor);
		m_free_objects.erase(itor);
		m_threadLock.Unlock();

		new(data->object) T();
		data->head.refCount = 1;
		m_threadLock.Lock();
		m_used_objects.insert(data);
		m_threadLock.Unlock();
		return data;
    }

	virtual bool IncRefCount(void * data, uint32 count)
	{
		ObjectData * objectData = (ObjectData*)data;
		m_threadLock.Lock();
		auto itor = m_used_objects.find(objectData);
		if (itor == m_used_objects.end())
		{
			m_threadLock.Unlock();
			assert(false);
			return false;
		}
		objectData->head.refCount += count;
		m_threadLock.Unlock();
		return true;
	}

	virtual bool DecRefCount(void * data, uint32 count)
	{
		ObjectData * objectData = (ObjectData*)data;
		m_threadLock.Lock();
		auto itor = m_used_objects.find(objectData);
		if (itor == m_used_objects.end())
		{
			m_threadLock.Unlock();
			assert(false);
			return false;
		}
		if (objectData->head.refCount > count)
		{
			objectData->head.refCount -= count;
			m_threadLock.Unlock();
			return true;
		}
		m_used_objects.erase(itor);
		objectData->head.refCount = 0;
		m_threadLock.Unlock();

		((T*)objectData->object)->~T();

		m_threadLock.Lock();
		m_free_objects.insert(objectData);
		m_threadLock.Unlock();
		return true;
	}
private:
	void assignObjs(uint32 count)
	{
		ObjectData * p = (ObjectData*)malloc(sizeof(ObjectData)*count);
		for (uint32 i = 0; i < count; ++i)
		{
			p[i].head.refCount = 0;
			p[i].head.objectDeleter = this;
			m_free_objects.insert(&p[i]);
		}
	}
	YSet<ObjectData*> m_used_objects;
	YSet<ObjectData*> m_free_objects;
	uint32 m_incCount;
	SpinLock m_threadLock;
};

YGAME_SERVER_END
