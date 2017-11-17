#include "../Common.h"

YGAME_SERVER_BEGIN

CoroutineGroup::CoroutineGroup()
{
}

CoroutineGroup::~CoroutineGroup()
{
}

void CoroutineGroup::Add(Coroutine * c)
{
	c->Run();
	if (c->IsRunning())
	{
		m_items.push_back(c);
	}
	else
	{
		gMemory.Delete(c);
	}
}

bool CoroutineGroup::Remove(Coroutine * c)
{
	for (int i=0; i<m_items.size(); ++i)
	{
		if (m_items[i] == c)
		{
			m_items.erase(m_items.begin()+i);
			return true;
		}
	}
	return false;
}

void CoroutineGroup::Update()
{
	for (auto itor = m_items.begin(); itor != m_items.end(); )
	{
		Coroutine * c = (*itor);
		if (!c->Run())
		{
			itor = m_items.erase(itor);
			gMemory.Delete(c);
		}
		else
		{
			++itor;
		}
	}
}

uint32 CoroutineGroup::GetCoroutineCount()
{
	return m_items.size();
}

YGAME_SERVER_END