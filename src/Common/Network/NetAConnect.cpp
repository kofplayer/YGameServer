#include "../Common.h"

YGAME_SERVER_BEGIN
NetAConnect::NetAConnect() : m_addr(0), m_port(0)
{
}

NetAConnect::~NetAConnect()
{
}

bool NetAConnect::Create(int type)
{
	return _create(type);
}

YGAME_SERVER_END