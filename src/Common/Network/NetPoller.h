YGAME_SERVER_BEGIN

class NetReadHandler
{
public:
	NetReadHandler() {}
	virtual ~NetReadHandler() {}
	virtual bool OnNetRead(SOCKET_ID s) = 0;
};

class NetWriteHandler
{
public:
	NetWriteHandler() {}
	virtual ~NetWriteHandler() {}
	virtual bool OnNetWrite(SOCKET_ID s) = 0;
};

class NetErrorHandler
{
public:
	NetErrorHandler() {}
	virtual ~NetErrorHandler() {}
	virtual bool OnNetError(SOCKET_ID s) = 0;
};

class NetPoller
{
public:
	NetPoller();
	virtual ~NetPoller();

	bool AddRead(SOCKET_ID s, NetReadHandler * handler);
	bool AddWrite(SOCKET_ID s, NetWriteHandler * handler);
	bool RemoveRead(SOCKET_ID s);
	bool RemoveWrite(SOCKET_ID s);

	virtual int32 WaitEvent(int32 waitUS) = 0;
protected:
	virtual bool DoAddRead(SOCKET_ID s) = 0;
	virtual bool DoAddWrite(SOCKET_ID s) = 0;
	virtual bool DoRemoveRead(SOCKET_ID s) = 0;
	virtual bool DoRemoveWrite(SOCKET_ID s) = 0;

	bool OnRead(SOCKET_ID s);
	bool OnWrite(SOCKET_ID s);
	bool OnError(SOCKET_ID s);
	bool IsAdded(SOCKET_ID s, bool isRead);
	int32 MaxFD() const;
private:
	YMap<SOCKET_ID, NetReadHandler*> m_readHandlers;
	YMap<SOCKET_ID, NetWriteHandler*> m_writeHandlers;
};


YGAME_SERVER_END
