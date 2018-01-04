YGAME_SERVER_BEGIN

class NetConnect : public NetBase
{
public:
	NetConnect();
	virtual ~NetConnect();
	virtual int32 Send(const void * data, int32 len);
	virtual int32 Recv(void * data, int32 len);

	virtual bool SendPacket(NetPacket * packet);
	virtual NetPacket * ReceivePacket();
	virtual bool DoSendPacket();
	virtual bool DoReceivePacket();

	template<typename T>
	void SetNetPacketer()
	{
		if (m_netPacketer)
		{
			gMemory.Delete(m_netPacketer);
		}
		m_netPacketer = gMemory.New<T>();
	}
protected:
	YList<NetPacket*> m_sendPackets;
	uint32 m_currentPacketSendedLen;
	NetPacketer * m_netPacketer;
};

YGAME_SERVER_END
