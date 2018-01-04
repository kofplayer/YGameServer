YGAME_SERVER_BEGIN

class NetPacketer
{
public:
	NetPacketer() {}
	virtual ~NetPacketer() {}
	virtual NetPacket * ReceivePacket() = 0;
	virtual bool OnReceiveData(const char * data, uint32 len) = 0;
};

template<typename PacketType, typename PacketHeadType>
class HeadNetPacketer : public NetPacketer
{
public:
	HeadNetPacketer() : m_currentPacket(NULL), m_currentPacketReceivedLen(0)
	{

	}
	virtual ~HeadNetPacketer()
	{

	}
	virtual NetPacket * ReceivePacket()
	{
		uint32 size = (uint32)m_packets.size();
		if (size <= 0)
		{
			return NULL;
		}
		NetPacket * packet = (*m_packets.begin());
		m_packets.pop_front();
		return packet;
	}
	virtual bool OnReceiveData(const char * data, uint32 len)
	{
		LOG_DEBUG("receive len %u\n", len);
		uint32 leftLen = len;
		uint32 doLen = 0;
		const char * p = data;
		uint32 headLen = sizeof(PacketHeadType);
		while (p && leftLen > 0)
		{
			// 读取消息头
			if (m_currentPacketReceivedLen < headLen)
			{
				doLen = headLen - m_currentPacketReceivedLen;
				if (doLen > len)
				{
					doLen = len;
				}
				memcpy((char*)(&m_head), p, doLen);
				leftLen -= doLen;
				p += doLen;
				m_currentPacketReceivedLen += doLen;

				// 消息头读取完整没有
				if (m_currentPacketReceivedLen >= headLen)
				{
					if (NULL != m_currentPacket)
					{
						assert(false);
						gMemory.Delete(m_currentPacket);
					}
					m_currentPacket = gMemory.New<PacketType>();
					uint32 dataLen = m_head.GetDataLen();
					m_currentPacket->Init(dataLen+headLen);
					memcpy(m_currentPacket->GetPacket(), &m_head, headLen);
					LOG_DEBUG("receive head finish, headLen:%u dataLen:%u\n", headLen, dataLen);
				}
			}
			//读取消息体
			if (m_currentPacket)
			{
				uint32 packetLen = m_currentPacket->GetPacketLen();
				if (m_currentPacketReceivedLen < packetLen)
				{
					char * packet = m_currentPacket->GetPacket();
					doLen = packetLen - m_currentPacketReceivedLen;
					if (doLen > len)
					{
						doLen = len;
					}
					memcpy(packet + m_currentPacketReceivedLen, p, doLen);
					leftLen -= doLen;
					p += doLen;
					m_currentPacketReceivedLen += doLen;

					
				}

				// 如果消息体读完了就加到消息列表里
				if (m_currentPacketReceivedLen >= packetLen)
				{
					m_currentPacketReceivedLen = 0;
					m_packets.push_back(m_currentPacket);
					m_currentPacket = NULL;
					LOG_DEBUG("receive packet finish, len:%u\n", packetLen);
				}
			}
		}
		return true;
	}
private:
	PacketHeadType m_head;
	YList<PacketType*> m_packets;
	PacketType * m_currentPacket;
	uint32 m_currentPacketReceivedLen;
};

YGAME_SERVER_END