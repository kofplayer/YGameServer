#include "../Common.h"

YGAME_SERVER_BEGIN
NetListener::NetListener()
{
}

NetListener::~NetListener()
{
}

bool NetListener::Create(int type)
{
	return _create(type);
}

bool NetListener::Bind(NET_ADDR addr, NET_PORT port)
{
	sockaddr_in	sin;
	memset(&sin, 0, sizeof(sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(addr);

	bool succ = (bind(m_socket, (struct sockaddr*)&sin, sizeof(sin)) == 0);
	if (succ)
	{
		m_addr = addr;
		m_port = port;
	}
	return succ;
}

bool NetListener::Listen(int backlog)
{
	return listen(m_socket, backlog) == 0;
}

NetPConnect * NetListener::Accept()
{
	sockaddr_in		sin;
	socklen_t		sinLen = sizeof(sin);
	SOCKET_ID s = (SOCKET_ID)accept(m_socket, (sockaddr*)&sin, &sinLen);

	if (s == INVALID_SOCKET) return NULL;

	NetPConnect * pNew = gMemory.New<NetPConnect>();
	pNew->SetSocket(s, sin.sin_addr.s_addr, sin.sin_port);
	return pNew;
}

YGAME_SERVER_END