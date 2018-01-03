YGAME_SERVER_BEGIN

struct NetPacketHead
{
	uint16 dataLen;
};

#define NetPacketHeadLen sizeof(NetPacketHead)

class NetPacket
{
public:
	NetPacket();
	virtual ~NetPacket();
	void Init(uint16 dataLen);
	void Uninit();
	char * GetPacket();
	uint16 GetPacketLen();
	char * GetData();
	uint16 GetDataLen();
protected:
	uint16 m_packetLen;
	char * m_packet;
};

class NetPacketSender
{
public:
	NetPacketSender() {}
	virtual ~NetPacketSender() {}
	virtual void SendPacket(SOCKET_ID s, NetPacket * packet) = 0;
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
	virtual NetPacketHandler * OnNetOpen(SOCKET_ID s, NetPacketSender * sender) = 0;
	virtual void OnNetClose(SOCKET_ID s) = 0;
};

YGAME_SERVER_END