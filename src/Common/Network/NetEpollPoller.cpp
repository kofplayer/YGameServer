#include "../Common.h"

YGAME_SERVER_BEGIN

NetEpollPoller::NetEpollPoller()
{
}

NetEpollPoller::~NetEpollPoller()
{
}

int32 NetEpollPoller::waitEvent(int32 waitUS)
{
	return 0;
}

bool NetEpollPoller::doAddRead(SOCKET_ID s)
{
	return false;
}

bool NetEpollPoller::doAddWrite(SOCKET_ID s)
{
	return false;
}

bool NetEpollPoller::doRemoveRead(SOCKET_ID s)
{
	return false;
}

bool NetEpollPoller::doRemoveWrite(SOCKET_ID s)
{
	return false;
}

YGAME_SERVER_END