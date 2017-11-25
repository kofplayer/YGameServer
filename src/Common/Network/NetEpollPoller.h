YGAME_SERVER_BEGIN

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
};

YGAME_SERVER_END