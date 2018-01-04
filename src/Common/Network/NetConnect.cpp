#include "../Common.h"

YGAME_SERVER_BEGIN
NetConnect::NetConnect() : m_currentPacketSendedLen(0), m_netPacketer(NULL)
{
}

NetConnect::~NetConnect()
{
	if (m_netPacketer)
	{
		gMemory.Delete(m_netPacketer);
		m_netPacketer = NULL;
		m_currentPacketSendedLen = 0;
	}
}

int32 NetConnect::Send(const void * data, int32 len)
{
	return send(m_socket, (char*)data, len, 0);
}

int32 NetConnect::Recv(void * data, int32 len)
{
	return recv(m_socket, (char*)data, len, 0);
}

bool NetConnect::SendPacket(NetPacket * packet)
{
	m_sendPackets.push_back(packet);
	if (m_sendPackets.size() == 1)
	{
		DoSendPacket();
	}
	return true;
}

NetPacket * NetConnect::ReceivePacket()
{
	return m_netPacketer->ReceivePacket();
}

bool NetConnect::DoSendPacket()
{
	bool bFirstSend = true;
	for (auto itor=m_sendPackets.begin(); itor!=m_sendPackets.end(); )
	{
		NetPacket * packet = *itor;
		uint32 packetLen = packet->GetPacketLen();
		if (m_currentPacketSendedLen >= packetLen)
		{
			assert(false);
			m_currentPacketSendedLen = 0;
			gMemory.Delete(packet);
			itor = m_sendPackets.erase(itor);
			continue;
		}
		int32 len = (int32)Send(packet->GetPacket()+m_currentPacketSendedLen, packetLen-m_currentPacketSendedLen);
		if (len == 0)
		{
			return !bFirstSend;
		}
		else if (len < 0)
		{
			return false;
		}
		m_currentPacketSendedLen += len;
		if (m_currentPacketSendedLen < packetLen)
		{
			return true;
		}
		m_currentPacketSendedLen = 0;
		gMemory.Delete(packet);
		itor = m_sendPackets.erase(itor);
	}
}

bool NetConnect::DoReceivePacket()
{
	char buff[512];
	bool bFirstRead = true;
	bool need_continue = true;
	while (need_continue)
	{
		int64 len = Recv(buff, sizeof(buff));
		char * p = buff;
		if (len == 0)
		{
			// 第一次就读取0表示客户端主动断开连接
			if (bFirstRead)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		// 小于0出错了
		else if (len < 0)
		{
			return false;
		}
		LOG_DEBUG("socket:%d receive len %llu\n", GetSocket(), len);
		bFirstRead = false;

		// 如果读入出来的长度不小于缓冲区大小表示可能还有数据需要读取
		need_continue = (len >= sizeof(buff));

		m_netPacketer->OnReceiveData(buff, len);
	}
	return true;
}



YGAME_SERVER_END
