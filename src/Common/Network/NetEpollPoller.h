YGAME_SERVER_BEGIN

#ifdef HAS_EPOLL
class NetEpollPoller : public NetPoller
{
public:
	NetEpollPoller();
	virtual ~NetEpollPoller();
	virtual int32 WaitEvent(int32 waitUS);
protected:
	virtual bool DoAddRead(SOCKET_ID s);
	virtual bool DoAddWrite(SOCKET_ID s);
	virtual bool DoRemoveRead(SOCKET_ID s);
	virtual bool DoRemoveWrite(SOCKET_ID s);
    bool DoAdd(SOCKET_ID s, bool isRead, bool isAdd);
private:
    int m_epfd;
};
#endif

YGAME_SERVER_END
