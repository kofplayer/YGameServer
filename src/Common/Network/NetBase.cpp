#include "../Common.h"

YGAME_SERVER_BEGIN
NetBase::NetBase() : m_socket(INVALID_SOCKET), m_addr(0), m_port(0)
{
}

NetBase::~NetBase()
{
}

SOCKET_ID NetBase::GetSocket()
{
	return m_socket;
}

NET_ADDR NetBase::GetAddr()
{
	return m_addr;
}

NET_PORT NetBase::GetPort()
{
	return m_port;
}

void NetBase::SetAddr(const NET_ADDR addr)
{
	m_addr = addr;
}

void NetBase::SetPort(const NET_PORT port)
{
	m_port = port;
}

void NetBase::Close()
{
	if (m_socket != INVALID_SOCKET)
	{
#if CURRENT_PLATFORM == PLATFORM_WIN32
		closesocket(m_socket);
#else
		::close(m_socket);
#endif
		m_socket = INVALID_SOCKET;
	}
}

bool NetBase::Create(int type, bool isBlock)
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
	bool succ = (m_socket != INVALID_SOCKET);
	if (succ && !isBlock)
	{
#if CURRENT_PLATFORM == PLATFORM_WIN32
		u_long nonblocking = 1;
		if (ioctlsocket(m_socket, FIONBIO, &nonblocking) == SOCKET_ERROR) 
		{
			assert(false);
		}
#else
		int flags = 0;
		if ((flags = fcntl(m_socket, F_GETFL, NULL)) < 0)
		{
			assert(false);
		}
		else if (fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) == -1)
		{
			assert(false);
		}
#endif
	}
	return succ;
}

YGAME_SERVER_END
