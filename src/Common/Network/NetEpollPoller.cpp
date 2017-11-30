#include "../Common.h"

YGAME_SERVER_BEGIN

#ifdef HAS_EPOLL

NetEpollPoller::NetEpollPoller() : m_epfd(epoll_create(10))
{
    if (m_epfd == -1)
    {
        LOG_ERROR("epoll_create failed: %s\n", getStrError());
    }
}

NetEpollPoller::~NetEpollPoller()
{
    if (m_epfd != -1)
    {
        close(m_epfd);
        m_epfd = -1;
    }
}

int32 NetEpollPoller::waitEvent(int32 waitUS)
{
    const int MAX_EVENTS = 10;
    struct epoll_event events[ MAX_EVENTS ];
    int32 maxWaitInMilliseconds = waitUS/1000;
    int nfds = epoll_wait(m_epfd, events, MAX_EVENTS, maxWaitInMilliseconds);
    for (int i = 0; i < nfds; ++i)
    {
        if (events[i].events & (EPOLLERR|EPOLLHUP))
        {
            onError(events[i].data.fd);
        }
        else
        {
            if (events[i].events & EPOLLIN)
            {
                onRead(events[i].data.fd);
            }
            if (events[i].events & EPOLLOUT)
            {
                onWrite(events[i].data.fd);
            }
        }
    }
    return nfds;
}

bool NetEpollPoller::doAddRead(SOCKET_ID s)
{
	return doAdd(s, true, true);
}

bool NetEpollPoller::doAddWrite(SOCKET_ID s)
{
	return doAdd(s, false, true);
}

bool NetEpollPoller::doRemoveRead(SOCKET_ID s)
{
	return doAdd(s, true, false);
}

bool NetEpollPoller::doRemoveWrite(SOCKET_ID s)
{
	return doAdd(s, false, false);
}

bool NetEpollPoller::doAdd(SOCKET_ID s, bool isRead, bool isAdd)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev)); // stop valgrind warning
    int op;
    ev.data.fd = s;
    // Handle the case where the file is already registered for the opposite
    // action.
    if (this->isAdded(s, !isRead))
    {
        op = EPOLL_CTL_MOD;
        ev.events = isAdd ? EPOLLIN|EPOLLOUT :
        isRead ? EPOLLOUT : EPOLLIN;
    }
    else
    {
        // TODO: Could be good to use EPOLLET (leave like select for now).
        ev.events = isRead ? EPOLLIN : EPOLLOUT;
        op = isAdd ? EPOLL_CTL_ADD : EPOLL_CTL_DEL;
    }
    if (epoll_ctl(epfd_, op, fd, &ev) < 0)
    {
        LOG_WARN("Failed to %s %s file socket %d, %s\n",
                 (isAdd ? "add" : "remove"),
                 (isRead ? "read" : "write"),
                 s,
                 getStrError());        return false;
    }
    return true;
}

#endif

YGAME_SERVER_END
