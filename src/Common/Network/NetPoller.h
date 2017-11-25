YGAME_SERVER_BEGIN

class NetReadListener
{
public:
	NetReadListener() {}
	virtual ~NetReadListener() {}
	virtual bool onNetRead(SOCKET_ID s) = 0;
};

class NetWriteListener
{
public:
	NetWriteListener() {}
	virtual ~NetWriteListener() {}
	virtual bool onNetWrite(SOCKET_ID s) = 0;
};

class NetErrorListener
{
public:
	NetErrorListener() {}
	virtual ~NetErrorListener() {}
	virtual bool onNetError(SOCKET_ID s) = 0;
};

class NetPoller
{
public:
	NetPoller();
	virtual ~NetPoller();

	bool addRead(SOCKET_ID s, NetReadListener * listener);
	bool addWrite(SOCKET_ID s, NetWriteListener * listener);
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

	int32 maxFD() const;
private:
	Map<SOCKET_ID, NetReadListener*> m_readListeners;
	Map<SOCKET_ID, NetWriteListener*> m_writeListeners;
};

YGAME_SERVER_END