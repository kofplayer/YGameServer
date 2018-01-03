#include "../Common.h"

YGAME_SERVER_BEGIN

NetSelectPoller::NetSelectPoller()
{
	FD_ZERO(&m_fdReadSet);
	FD_ZERO(&m_fdWriteSet);
	m_fdLargest = -1;
	m_fdWriteCount = 0;
}

NetSelectPoller::~NetSelectPoller()
{
}

int32 NetSelectPoller::WaitEvent(int32 waitUS)
{
	fd_set readFDs;
	fd_set writeFDs;
	struct timeval nextTimeout;

	FD_ZERO(&readFDs);
	FD_ZERO(&writeFDs);

	readFDs = m_fdReadSet;
	writeFDs = m_fdWriteSet;

	nextTimeout.tv_sec = waitUS / 1000000;
	nextTimeout.tv_usec = waitUS % 1000000;

	int32 countReady = 0;

#if CURRENT_PLATFORM == PLATFORM_WIN32
	if (m_fdLargest == -1)
	{
		// Windows can't handle it if we don't have any FDs to select on, but
		// we have a non-NULL timeout.
		Sleep(waitUS / 1000);
	}
	else
#endif
	{
		countReady = select(m_fdLargest + 1, &readFDs,
			m_fdWriteCount ? &writeFDs : NULL, NULL, &nextTimeout);
	}

	if (countReady > 0)
	{
		this->HandleEvent(countReady, readFDs, writeFDs);
	}
	else if (countReady == -1)
	{
		LOG_WARN("error in select(): %s\n", getStrError());
	}

	return countReady;
}

bool NetSelectPoller::DoAddRead(SOCKET_ID fd)
{
#if CURRENT_PLATFORM != PLATFORM_WIN32
	if ((fd < 0) || (FD_SETSIZE <= fd))
	{
		LOG_ERROR("tried to register invalid fd %d. FD_SETSIZE (%d)\n", fd, FD_SETSIZE);
		return false;
	}
#else
	if (m_fdReadSet.fd_count >= FD_SETSIZE)
	{
		LOG_ERROR("tried to register invalid fd %d. FD_SETSIZE (%d)\n", fd, FD_SETSIZE);
		return false;
	}
#endif

	if (FD_ISSET(fd, &m_fdReadSet))
	{
		return false;
	}

	FD_SET(fd, &m_fdReadSet);

	if (fd > m_fdLargest)
	{
		m_fdLargest = fd;
	}

	return true;
}

bool NetSelectPoller::DoAddWrite(SOCKET_ID fd)
{
#if CURRENT_PLATFORM != PLATFORM_WIN32
	if ((fd < 0) || (FD_SETSIZE <= fd))
	{
		LOG_ERROR("tried to register invalid fd %d. FD_SETSIZE (%d)\n", fd, FD_SETSIZE);
		return false;
	}
#else
	if (m_fdWriteSet.fd_count >= FD_SETSIZE)
	{
		LOG_ERROR("tried to register invalid fd %d. FD_SETSIZE (%d)\n", fd, FD_SETSIZE);
		return false;
	}
#endif

	if (FD_ISSET(fd, &m_fdWriteSet))
	{
		return false;
	}

	FD_SET(fd, &m_fdWriteSet);

	if (fd > m_fdLargest)
	{
		m_fdLargest = fd;
	}
	++m_fdWriteCount;
	return true;
}

bool NetSelectPoller::DoRemoveRead(SOCKET_ID fd)
{
#if CURRENT_PLATFORM != PLATFORM_WIN32
	if ((fd < 0) || (FD_SETSIZE <= fd))
	{
		return false;
	}
#endif
	if (!FD_ISSET(fd, &m_fdReadSet))
	{
		return false;
	}

	FD_CLR(fd, &m_fdReadSet);

	if (fd == m_fdLargest)
	{
		m_fdLargest = MaxFD();
	}

	return true;
}

bool NetSelectPoller::DoRemoveWrite(SOCKET_ID fd)
{
#if CURRENT_PLATFORM != PLATFORM_WIN32
	if ((fd < 0) || (FD_SETSIZE <= fd))
	{
		return false;
	}
#endif
	if (!FD_ISSET(fd, &m_fdWriteSet))
	{
		return false;
	}

	FD_CLR(fd, &m_fdWriteSet);

	if (fd == m_fdLargest)
	{
		m_fdLargest = MaxFD();
	}
	--m_fdWriteCount;
	return true;
}

void NetSelectPoller::HandleEvent(int32 &countReady, fd_set &readFDs, fd_set &writeFDs)
{
#if CURRENT_PLATFORM == PLATFORM_WIN32
		for (unsigned i = 0; i < readFDs.fd_count; ++i)
		{
			int fd = (int)readFDs.fd_array[i];
			--countReady;
			OnRead(fd);
		}

		for (unsigned i = 0; i < writeFDs.fd_count; ++i)
		{
			int fd = (int)writeFDs.fd_array[i];
			--countReady;
			OnWrite(fd);
		}

#else
		for (int fd = 0; fd <= m_fdLargest && countReady > 0; ++fd)
		{
			if (FD_ISSET(fd, &readFDs))
			{
				--countReady;
				OnRead(fd);
			}

			if (FD_ISSET(fd, &writeFDs))
			{
				--countReady;
				OnWrite(fd);
			}
		}
#endif
}

YGAME_SERVER_END