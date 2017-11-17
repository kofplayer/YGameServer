YGAME_SERVER_BEGIN

class ObjectDeleter
{
public:
	ObjectDeleter() {}
	virtual ~ObjectDeleter() {}
	virtual void Delete(void * data) = 0;
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
		ObjectDeleter * objectDeleter;
		char object[sizeof(T)];
	};
#pragma pack()
	ObjectPool()
	{
		m_inc_count = 16;
	}
	void Init(uint32 init_count, uint32 inc_count)
	{
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
		if (m_free_objects.size()==0)
		{
			assignObjs(m_inc_count);
		}
		ObjectData* data = (*m_free_objects.begin());
		m_free_objects.pop_front();
		m_used_objects.insert(data);
		new(data->object) T();
		return data;
    }
    
	virtual void Delete(void * data)
    {
		auto itor = m_used_objects.find((ObjectData*)data);
		if (itor == m_used_objects.end())
		{
			assert(false);
			return;
		}
		((T*)((*itor)->object))->~T();
		m_free_objects.push_back(*itor);
		m_used_objects.erase(itor);
	}
private:
	void assignObjs(uint32 count)
	{
		ObjectData * p = (ObjectData*)malloc(sizeof(ObjectData)*count);
		for (unsigned int i = 0; i < count; ++i)
		{
			p[i].objectDeleter = this;
			m_free_objects.push_back(&p[i]);
		}
	}
	Set<ObjectData*> m_used_objects;
	List<ObjectData*> m_free_objects;
	uint32 m_inc_count;
};

YGAME_SERVER_END
