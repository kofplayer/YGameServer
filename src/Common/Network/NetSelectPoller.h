YGAME_SERVER_BEGIN

class NetSelectPoller : public NetPoller
{
public:
	NetSelectPoller();
	virtual ~NetSelectPoller();
	virtual int32 waitEvent(int32 waitUS);
protected:
	virtual bool doAddRead(SOCKET_ID fd);
	virtual bool doAddWrite(SOCKET_ID fd);
	virtual bool doRemoveRead(SOCKET_ID fd);
	virtual bool doRemoveWrite(SOCKET_ID fd);

private:
	void handleEvent(int32 &countReady, fd_set &readFDs, fd_set &writeFDs);

	fd_set m_fdReadSet;
	fd_set m_fdWriteSet;
	int32 m_fdLargest;
	int32 m_fdWriteCount;
};

YGAME_SERVER_END