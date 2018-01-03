YGAME_SERVER_BEGIN

class NetSelectPoller : public NetPoller
{
public:
	NetSelectPoller();
	virtual ~NetSelectPoller();
	virtual int32 WaitEvent(int32 waitUS);
protected:
	virtual bool DoAddRead(SOCKET_ID fd);
	virtual bool DoAddWrite(SOCKET_ID fd);
	virtual bool DoRemoveRead(SOCKET_ID fd);
	virtual bool DoRemoveWrite(SOCKET_ID fd);

private:
	void HandleEvent(int32 &countReady, fd_set &readFDs, fd_set &writeFDs);

	fd_set m_fdReadSet;
	fd_set m_fdWriteSet;
	int32 m_fdLargest;
	int32 m_fdWriteCount;
};

YGAME_SERVER_END