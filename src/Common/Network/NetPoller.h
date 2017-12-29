YGAME_SERVER_BEGIN

class NetReadHandler
{
public:
	NetReadHandler() {}
	virtual ~NetReadHandler() {}
	virtual bool onNetRead(SOCKET_ID s) = 0;
};

class NetWriteHandler
{
public:
	NetWriteHandler() {}
	virtual ~NetWriteHandler() {}
	virtual bool onNetWrite(SOCKET_ID s) = 0;
};

class NetErrorHandler
{
public:
	NetErrorHandler() {}
	virtual ~NetErrorHandler() {}
	virtual bool onNetError(SOCKET_ID s) = 0;
};

class NetPoller
{
public:
	NetPoller();
	virtual ~NetPoller();

	bool addRead(SOCKET_ID s, NetReadHandler * handler);
	bool addWrite(SOCKET_ID s, NetWriteHandler * handler);
	bool removeRead(SOCKET_ID s);
	bool removeWrite(SOCKET_ID s);

	virtual int32 waitEvent(int32 waitUS) = 0;
protected:
	virtual bool doAddRead(SOCKET_ID s) = 0;
	virtual bool doAddWrite(SOCKET_ID s) = 0;
	virtual bool doRemoveRead(SOCKET_ID s) = 0;
	virtual bool doRemoveWrite(SOCKET_ID s) = 0;

	bool onRead(SOCKET_ID s);
	bool onWrite(SOCKET_ID s);
    bool onError(SOCKET_ID s);
    bool isAdded(SOCKET_ID s, bool isRead);
	int32 maxFD() const;
private:
	YMap<SOCKET_ID, NetReadHandler*> m_readHandlers;
	YMap<SOCKET_ID, NetWriteHandler*> m_writeHandlers;
	SpinLock m_lock;
};


YGAME_SERVER_END
