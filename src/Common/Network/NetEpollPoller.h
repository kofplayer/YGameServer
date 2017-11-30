YGAME_SERVER_BEGIN

#ifdef HAS_EPOLL
class NetEpollPoller : public NetPoller
{
public:
	NetEpollPoller();
	virtual ~NetEpollPoller();
	virtual int32 waitEvent(int32 waitUS);
protected:
	virtual bool doAddRead(SOCKET_ID s);
	virtual bool doAddWrite(SOCKET_ID s);
	virtual bool doRemoveRead(SOCKET_ID s);
	virtual bool doRemoveWrite(SOCKET_ID s);
    bool doAdd(SOCKET_ID s, bool isRead, bool isAdd);
private:
    int m_epfd;
};
#endif

YGAME_SERVER_END
