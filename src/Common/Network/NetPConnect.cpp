#include "../Common.h"

YGAME_SERVER_BEGIN
NetPConnect::NetPConnect()
{
}

NetPConnect::~NetPConnect()
{
}

void NetPConnect::SetSocket(SOCKET_ID s, NET_ADDR addr, NET_PORT port)
{
	assert(m_socket == INVALID_SOCKET);
	m_socket = s;
	m_addr = addr;
	m_port = port;
}

YGAME_SERVER_END