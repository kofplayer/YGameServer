YGAME_SERVER_BEGIN

class NetPacketWarp : public NetReadHandler, public NetWriteHandler, public NetPacketSender
{
public:
	NetPacketWarp();
	virtual ~NetPacketWarp();

	virtual bool OnNetRead(SOCKET_ID s);
	virtual bool OnNetWrite(SOCKET_ID s);
	virtual void SendPacket(SOCKET_ID s, NetPacket * packet);

	bool Listen(NET_ADDR addr, NET_PORT port, NetStatusHandler * handler);
	bool Connect(NET_ADDR addr, NET_PORT port, NetStatusHandler * handler);
	int32 WaitEvent(int32 waitUS);
protected:
	void DoSendPacket(SOCKET_ID s);
	void OnNetOpen(SOCKET_ID s, SOCKET_ID listenSocket);
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
	struct NetPacketReceiveInfo
	{
		uint16 didHeadLen;
		char headData[NetPacketHeadLen];
		uint16 didDataLen;
		NetPacket * packet;
		NetPacketReceiveInfo()
		{
			didHeadLen = 0;
			didDataLen = 0;
			packet = NULL;
		}
		~NetPacketReceiveInfo()
		{
			if (packet)
			{
				gMemory.Delete(packet);
				packet = NULL;
			}
		}
		bool IsFinish()
		{
			return didHeadLen >= sizeof(headData) && (packet == NULL ? true : didDataLen >= packet->GetDataLen());
		}
	};
	struct NetPacketSendInfo
	{
		uint16 didLen;
		NetPacket * packet;
		NetPacketSendInfo()
		{
			didLen = 0;
			packet = NULL;
		}
		~NetPacketSendInfo()
		{
			if (packet)
			{
				gMemory.Delete(packet);
				packet = NULL;
			}
		}
		bool IsFinish()
		{
			if (NULL == packet)
			{
				return true;
			}
			return didLen >= packet->GetPacketLen();
		}
	};
	typedef YList<NetPacketReceiveInfo*> PacketReceiveInfoList;
	typedef YList<NetPacketSendInfo*> PacketSendInfoList;
	struct NetInfo
	{
		NetConnect * connect;
		NetPacketHandler * handler;
		PacketReceiveInfoList receives;
		PacketSendInfoList sends;
		NetInfo()
		{
			connect = NULL;
			handler = NULL;
		}
		~NetInfo()
		{
			for (auto itor = receives.begin(); itor != receives.end(); ++itor)
			{
				gMemory.Delete(*itor);
			}
			receives.clear();
			for (auto itor = sends.begin(); itor != sends.end(); ++itor)
			{
				gMemory.Delete(*itor);
			}
			sends.clear();
			if (connect)
			{
				gMemory.Delete(connect);
				connect = NULL;
			}
		}
	};
	typedef YMap<SOCKET_ID, NetInfo*> NetInfos;
	NetInfos m_netInfos;
};

YGAME_SERVER_END