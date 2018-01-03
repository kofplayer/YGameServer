#include "../../Common.h"

YGAME_SERVER_BEGIN

NetPacket::NetPacket() : m_packetLen(0), m_packet(NULL)
{
}

NetPacket::~NetPacket()
{
	Uninit();
}

void NetPacket::Init(uint16 dataLen)
{
	Uninit();
	m_packetLen = dataLen+NetPacketHeadLen;
	m_packet = (char*)gMemory.Malloc(m_packetLen);
	((NetPacketHead*)m_packet)->dataLen = dataLen;
}

void NetPacket::Uninit()
{
	if (m_packet)
	{
		gMemory.Free(m_packet);
		m_packet = NULL;
		m_packetLen = 0;
	}
}

char * NetPacket::GetPacket()
{
	return m_packet;
}

uint16 NetPacket::GetPacketLen()
{
	return m_packetLen;
}

char * NetPacket::GetData()
{
	if (m_packet)
	{
		return m_packet+NetPacketHeadLen;
	}
	return NULL;
}

uint16 NetPacket::GetDataLen()
{
	if (m_packetLen < NetPacketHeadLen)
	{
		return 0;
	}
	return m_packetLen-NetPacketHeadLen;
}

YGAME_SERVER_END
