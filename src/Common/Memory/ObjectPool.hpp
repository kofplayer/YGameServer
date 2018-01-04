YGAME_SERVER_BEGIN

class ObjectDeleter
{
public:
	ObjectDeleter() {}
	virtual ~ObjectDeleter() {}
	virtual bool IncRefCount(void * data, uint32 count) = 0;
	virtual bool DecRefCount(void * data, uint32 count) = 0;
};

struct ObjectDataHead
{
	uint32 refCount;
	ObjectDeleter * objectDeleter;
};

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
		m_inc_count = 16;
	}
	void Init(uint32 init_count, uint32 inc_count)
	{
		AutoThreadLock autoLock(&m_threadLock);
		if (inc_count != 0)
		{
			m_inc_count = inc_count;
		}
		if (m_used_objects.size() == 0 && m_free_objects.size() == 0)
		{
			assignObjs(init_count);
		}
	}

	ObjectData * New()
    {
		m_threadLock.Lock();
		if (m_free_objects.size()==0)
		{
			assignObjs(m_inc_count);
		}
		ObjectData* data = (*m_free_objects.begin());
		m_free_objects.pop_front();
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
		m_free_objects.push_back(objectData);
		m_threadLock.Unlock();
		return true;
	}
private:
	void assignObjs(uint32 count)
	{
		ObjectData * p = (ObjectData*)malloc(sizeof(ObjectData)*count);
		for (unsigned int i = 0; i < count; ++i)
		{
			p[i].head.refCount = 0;
			p[i].head.objectDeleter = this;
			m_free_objects.push_back(&p[i]);
		}
	}
	YSet<ObjectData*> m_used_objects;
	YList<ObjectData*> m_free_objects;
	uint32 m_inc_count;
	SpinLock m_threadLock;
};

YGAME_SERVER_END
