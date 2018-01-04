YGAME_SERVER_BEGIN

class NetPacketWarp : public NetReadHandler, public NetWriteHandler
{
public:
	NetPacketWarp();
	virtual ~NetPacketWarp();

	virtual bool OnNetRead(SOCKET_ID s);
	virtual bool OnNetWrite(SOCKET_ID s);

	bool Listen(NET_ADDR addr, NET_PORT port, NetStatusHandler * handler);
	bool Connect(NET_ADDR addr, NET_PORT port, NetStatusHandler * handler);
	int32 WaitEvent(int32 waitUS);
protected:
	void DoSendPacket(SOCKET_ID s);
	void OnNetOpen(NetConnect * connect, SOCKET_ID listenSocket);
	void OnNetClose(SOCKET_ID s);
	void OnNetPacket(SOCKET_ID s, NetPacket * packet);
#ifdef HAS_EPOLL
	NetEpollPoller m_net_poller;
#else
	NetSelectPoller m_net_poller;
#endif
	typedef YMap<SOCKET_ID, NetStatusHandler*> NetStatusHandlerMap;
	YMap<SOCKET_ID, NetListener*> m_netListeners;
	YMap<SOCKET_ID, NetAConnect*> m_netConnecters;
	NetStatusHandlerMap m_netStatusHandlers;
	struct NetInfo
	{
		NetConnect * connect;
		NetPacketHandler * handler;
		NetInfo()
		{
			connect = NULL;
			handler = NULL;
		}
	};
	typedef YMap<SOCKET_ID, NetInfo*> NetInfos;
	NetInfos m_netInfos;
};

YGAME_SERVER_END