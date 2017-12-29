#include "../Common.h"

YGAME_SERVER_BEGIN

NetPoller::NetPoller()
{
}

NetPoller::~NetPoller()
{
}

bool NetPoller::addRead(SOCKET_ID s, NetReadHandler * handler)
{
	AutoThreadLock autoLock(&m_lock);
	if (!doAddRead(s))
	{
		return false;
	}
	m_readHandlers[s] = handler;
	return true;
}

bool NetPoller::addWrite(SOCKET_ID s, NetWriteHandler * handler)
{
	AutoThreadLock autoLock(&m_lock);
	if (!doAddWrite(s))
	{
		return false;
	}
	m_writeHandlers[s] = handler;
	return false;
}

bool NetPoller::removeRead(SOCKET_ID s)
{
	AutoThreadLock autoLock(&m_lock);
	if (!doRemoveRead(s))
	{
		return false;
	}
	m_readHandlers.erase(s);
	return true;
}

bool NetPoller::removeWrite(SOCKET_ID s)
{
	AutoThreadLock autoLock(&m_lock);
	if (!doRemoveWrite(s))
	{
		return false;
	}
	m_writeHandlers.erase(s);
	return true;
}

bool NetPoller::onRead(SOCKET_ID s)
{
	AutoThreadLock autoLock(&m_lock);
	auto itor = m_readHandlers.find(s);
	if (itor == m_readHandlers.end())
	{
		return false;
	}
	return itor->second->onNetRead(s);
}

bool NetPoller::onWrite(SOCKET_ID s)
{
	AutoThreadLock autoLock(&m_lock);
	auto itor = m_writeHandlers.find(s);
	if (itor == m_writeHandlers.end())
	{
		return false;
	}
	return itor->second->onNetWrite(s);
}

bool NetPoller::onError(SOCKET_ID s)
{
    if (!onRead(s))
    {
        return onWrite(s);
    }
    return true;
}

bool NetPoller::isAdded(SOCKET_ID s, bool isRead)
{
    return isRead ? m_readHandlers.find(s) != m_readHandlers.end() : m_writeHandlers.find(s) != m_writeHandlers.end();;
}

int32 NetPoller::maxFD() const
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
