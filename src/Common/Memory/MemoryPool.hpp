YGAME_SERVER_BEGIN

class MemoryPool;
#pragma pack(1)
struct MemoryDataHead
{
	MemoryPool * memoryPool;
};
struct MemoryData
{
	MemoryDataHead head;
	char memory[1];
};
#pragma pack()

class MemoryPool
{
	friend class MemoryManager;
	friend class ObjectPool<MemoryPool>;
protected:
	MemoryPool() : m_memorySize(0), m_incCount(4)
	{

	}
	void Init(uint32 memorySize, uint32 initCount, uint32 incCount)
	{
		AutoThreadLock autoLock(&m_threadLock);
		if (incCount != 0)
		{
			m_incCount = incCount;
		}
		if (m_usedMemory.size() == 0 && m_freeMemory.size() == 0)
		{
			m_memorySize = memorySize;
			assert(memorySize > 0);
			assignObjs(initCount);
		}
	}
	MemoryData * Malloc()
	{
		AutoThreadLock autoLock(&m_threadLock);
		if (m_memorySize <= 0)
		{
			assert(false);
			return NULL;
		}
		if (m_freeMemory.empty())
		{
			assignObjs(m_incCount);
		}
		auto itor = m_freeMemory.begin();
		MemoryData * data = *itor;
		m_freeMemory.erase(itor);
		m_usedMemory.insert(data);
		return data;
	}
	bool Free(MemoryData * data)
	{
		AutoThreadLock autoLock(&m_threadLock);
		auto itor = m_usedMemory.find(data);
		if (itor == m_usedMemory.end())
		{
			assert(false);
			return false;
		}
		m_usedMemory.erase(itor);
		m_freeMemory.insert(data);
		return true;
	}
private:
	void assignObjs(uint32 count)
	{
		uint32 dataSize = sizeof(MemoryDataHead)+m_memorySize;
		char * p = (char*)malloc(dataSize*count);
		for (uint32 i = 0; i < count; ++i)
		{
			MemoryDataHead * head = (MemoryDataHead*)p;
			head->memoryPool = this;
			m_freeMemory.insert((MemoryData*)p);
			p += dataSize;
		}
	}
	YSet<MemoryData*> m_freeMemory;
	YSet<MemoryData*> m_usedMemory;
	uint32 m_memorySize;
	uint32 m_incCount;
	SpinLock m_threadLock;
};

YGAME_SERVER_END
