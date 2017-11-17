#include "../Common.h"

YGAME_SERVER_BEGIN
NetBase::NetBase() : m_socket(INVALID_SOCKET), m_addr(0), m_port(0)
{
}

NetBase::~NetBase()
{
}

NET_ADDR NetBase::GetAddr()
{
	return m_addr;
}

NET_PORT NetBase::GetPort()
{
	return m_port;
}

bool NetBase::_create(int type)
{
	m_socket = (SOCKET_ID)socket(AF_INET, type, 0);
#if CURRENT_PLATFORM == PLATFORM_WIN32
	if ((m_socket == INVALID_SOCKET) && (WSAGetLastError() == WSANOTINITIALISED))
	{
		WSAData wsdata;
		WSAStartup(0x202, &wsdata);
		m_socket = (SOCKET_ID)socket(AF_INET, type, 0);
	}
#endif
	return (m_socket != INVALID_SOCKET);
}

YGAME_SERVER_END