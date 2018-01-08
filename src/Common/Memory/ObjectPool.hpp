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
		ObjectData* data = NULL;
		{
			AutoThreadLock autoLock(&m_threadLock);
			if (m_free_objects.size()==0)
			{
				assignObjs(m_incCount);
			}
			auto itor = m_free_objects.begin();
			data = (*itor);
			m_free_objects.erase(itor);
		}
		new(data->object) T();
		{
			AutoThreadLock autoLock(&m_threadLock);
			data->head.refCount = 1;
			m_used_objects.insert(data);
		}
		return data;
    }

	virtual bool IncRefCount(void * data, uint32 count)
	{
		AutoThreadLock autoLock(&m_threadLock);
		ObjectData * objectData = (ObjectData*)data;
		auto itor = m_used_objects.find(objectData);
		if (itor == m_used_objects.end())
		{
			assert(false);
			return false;
		}
		objectData->head.refCount += count;
		return true;
	}

	virtual bool DecRefCount(void * data, uint32 count)
	{
		ObjectData * objectData = (ObjectData*)data;
		{
			AutoThreadLock autoLock(&m_threadLock);
			auto itor = m_used_objects.find(objectData);
			if (itor == m_used_objects.end())
			{
				assert(false);
				return false;
			}
			if (objectData->head.refCount > count)
			{
				objectData->head.refCount -= count;
				return true;
			}
			m_used_objects.erase(itor);
			objectData->head.refCount = 0;
		}

		((T*)objectData->object)->~T();

		{
			AutoThreadLock autoLock(&m_threadLock);
			m_free_objects.insert(objectData);
		}
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
