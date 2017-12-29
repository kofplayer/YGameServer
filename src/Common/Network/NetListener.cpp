#include "../Common.h"

YGAME_SERVER_BEGIN
NetListener::NetListener()
{
}

NetListener::~NetListener()
{
}

bool NetListener::Bind()
{
	sockaddr_in	sin;
	memset(&sin, 0, sizeof(sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_port);
	sin.sin_addr.s_addr = htonl(m_addr);
	return (bind(m_socket, (struct sockaddr*)&sin, sizeof(sin)) == 0);
}

bool NetListener::Listen(int backlog)
{
	Close();
	if (!Create(SOCK_STREAM, true))
	{
		return false;
	}
	if (!Bind())
	{
		return false;
	}
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