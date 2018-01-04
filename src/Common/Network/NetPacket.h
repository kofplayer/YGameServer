YGAME_SERVER_BEGIN

class NetConnect;

class NetPacket
{
public:
	NetPacket();
	virtual ~NetPacket();
	void Init(uint32 len);
	void Uninit();
	char * GetPacket();
	uint32 GetPacketLen();
protected:
	uint32 m_packetLen;
	char * m_packet;
};

class NetPacketHandler
{
public:
	NetPacketHandler() {}
	virtual ~NetPacketHandler() {}
	virtual void OnNetPacket(SOCKET_ID s, NetPacket * packet) = 0;
};

class NetStatusHandler
{
public:
	NetStatusHandler() {}
	virtual ~NetStatusHandler() {}
	virtual NetPacketHandler * OnNetOpen(NetConnect * connect) = 0;
	virtual void OnNetClose(SOCKET_ID s) = 0;
};

YGAME_SERVER_END