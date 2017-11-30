#include "../Common.h"

YGAME_SERVER_BEGIN

NetPoller::NetPoller()
{
}

NetPoller::~NetPoller()
{
}

bool NetPoller::addRead(SOCKET_ID s, NetReadListener * listener)
{
	if (!doAddRead(s))
	{
		return false;
	}
	m_readListeners[s] = listener;
	return true;
}

bool NetPoller::addWrite(SOCKET_ID s, NetWriteListener * listener)
{
	if (!doAddWrite(s))
	{
		return false;
	}
	m_writeListeners[s] = listener;
	return false;
}

bool NetPoller::removeRead(SOCKET_ID s)
{
	if (!doRemoveRead(s))
	{
		return false;
	}
	m_readListeners.erase(s);
	return true;
}

bool NetPoller::removeWrite(SOCKET_ID s)
{
	if (!doRemoveWrite(s))
	{
		return false;
	}
	m_writeListeners.erase(s);
	return true;
}

bool NetPoller::onRead(SOCKET_ID s)
{
	auto itor = m_readListeners.find(s);
	if (itor == m_readListeners.end())
	{
		return false;
	}
	return itor->second->onNetRead(s);
}

bool NetPoller::onWrite(SOCKET_ID s)
{
	auto itor = m_writeListeners.find(s);
	if (itor == m_writeListeners.end())
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
    return isRead ? m_readListeners.find(s) != m_readListeners.end() : m_writeListeners.find(s) != m_writeListeners.end();;
}

int32 NetPoller::maxFD() const
{
	int32 max = -1;

	for (auto itor = m_readListeners.begin();  itor != m_readListeners.end(); ++itor)
	{
		if (itor->first > max)
		{
			max = itor->first;
		}
	}

	for (auto itor = m_writeListeners.begin(); itor != m_writeListeners.end(); ++itor)
	{
		if (itor->first > max)
		{
			max = itor->first;
		}
	}
	return max;
}


YGAME_SERVER_END
