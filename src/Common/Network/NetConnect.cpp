#include "../Common.h"

YGAME_SERVER_BEGIN
NetConnect::NetConnect()
{
}

NetConnect::~NetConnect()
{
}

int32 NetConnect::Send(const void * data, int32 len)
{
	return send(m_socket, (char*)data, len, 0);
}

int32 NetConnect::Recv(void * data, int32 len)
{
	return recv(m_socket, (char*)data, len, 0);
}


YGAME_SERVER_END