#include "../Common.h"

YGAME_SERVER_BEGIN

NetPacket::NetPacket() : m_packetLen(0), m_packet(NULL)
{
}

NetPacket::~NetPacket()
{
	Uninit();
}

void NetPacket::Init(uint32 len)
{
	Uninit();
	m_packetLen = len;
	m_packet = (char*)gMemory.Malloc(m_packetLen);
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

uint32 NetPacket::GetPacketLen()
{
	return m_packetLen;
}

YGAME_SERVER_END
