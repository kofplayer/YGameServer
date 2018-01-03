#include "../Common.h"

YGAME_SERVER_BEGIN

NetPoller::NetPoller()
{
}

NetPoller::~NetPoller()
{
}

bool NetPoller::AddRead(SOCKET_ID s, NetReadHandler * handler)
{
	AutoThreadLock autoLock(&m_lock);
	if (!DoAddRead(s))
	{
		return false;
	}
	m_readHandlers[s] = handler;
	return true;
}

bool NetPoller::AddWrite(SOCKET_ID s, NetWriteHandler * handler)
{
	AutoThreadLock autoLock(&m_lock);
	if (!DoAddWrite(s))
	{
		return false;
	}
	m_writeHandlers[s] = handler;
	return false;
}

bool NetPoller::RemoveRead(SOCKET_ID s)
{
	AutoThreadLock autoLock(&m_lock);
	m_readHandlers.erase(s);
	if (!DoRemoveRead(s))
	{
		return false;
	}
	return true;
}

bool NetPoller::RemoveWrite(SOCKET_ID s)
{
	AutoThreadLock autoLock(&m_lock);
	m_writeHandlers.erase(s);
	if (!DoRemoveWrite(s))
	{
		return false;
	}
	return true;
}

bool NetPoller::OnRead(SOCKET_ID s)
{
	AutoThreadLock autoLock(&m_lock);
	auto itor = m_readHandlers.find(s);
	if (itor == m_readHandlers.end())
	{
		return false;
	}
	return itor->second->OnNetRead(s);
}

bool NetPoller::OnWrite(SOCKET_ID s)
{
	AutoThreadLock autoLock(&m_lock);
	auto itor = m_writeHandlers.find(s);
	if (itor == m_writeHandlers.end())
	{
		return false;
	}
	return itor->second->OnNetWrite(s);
}

bool NetPoller::OnError(SOCKET_ID s)
{
    if (!OnRead(s))
    {
        return OnWrite(s);
    }
    return true;
}

bool NetPoller::IsAdded(SOCKET_ID s, bool isRead)
{
    return isRead ? m_readHandlers.find(s) != m_readHandlers.end() : m_writeHandlers.find(s) != m_writeHandlers.end();;
}

int32 NetPoller::MaxFD() const
{
	int32 max = -1;

	for (auto itor = m_readHandlers.begin();  itor != m_readHandlers.end(); ++itor)
	{
		if (itor->first > max)
		{
			max = itor->first;
		}
	}

	for (auto itor = m_writeHandlers.begin(); itor != m_writeHandlers.end(); ++itor)
	{
		if (itor->first > max)
		{
			max = itor->first;
		}
	}
	return max;
}


YGAME_SERVER_END
