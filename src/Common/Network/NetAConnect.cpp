#include "../Common.h"

YGAME_SERVER_BEGIN
NetAConnect::NetAConnect()
{
}

NetAConnect::~NetAConnect()
{
}

bool NetAConnect::Connect(bool isBlock)
{
	Close();
	Create(SOCK_STREAM, isBlock);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = htonl(m_addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);

	int nRet = ::connect(m_socket, (struct sockaddr*)&addr, sizeof(addr));
	if (nRet == SOCKET_ERROR)
	{
		if (!isBlock)
		{
			int error = getLastError();
#if CURRENT_PLATFORM == PLATFORM_APPLE
            if (1 == error)
#else
            if (EINPROGRESS == error || EWOULDBLOCK ==  error || WSAEWOULDBLOCK ==  error)
#endif
			{
				return true;
			}
		}
	}
	else
	{
		return true;
	}

	return false;
}

bool NetAConnect::ConnectUrl(const char * url)
{
#if CURRENT_PLATFORM != PLATFORM_WIN32
    return false;
#else
	Close();
	addrinfo * qresult = NULL;
	addrinfo qhints;
	addrinfo * qres = NULL;
	memset(&qhints, 0, sizeof(qhints));
	qhints.ai_family = PF_UNSPEC;
	qhints.ai_socktype = SOCK_STREAM;
	qhints.ai_protocol = IPPROTO_TCP;
	char qport[32] = {0};
	snprintf(qport, sizeof(qport)-1, "%u", m_port);
	int qerror = getaddrinfo(url, qport, &qhints, &qresult);
	if (qerror != 0 || NULL == qresult)
	{
		return false;
	}

	bool IsConnect = false;
	for (qres = qresult; qres != NULL; qres = qres->ai_next)
	{
		m_socket = socket(qres->ai_family, qres->ai_socktype, qres->ai_protocol);
		if( m_socket == INVALID_SOCKET )
		{
			continue;
		}
		if (::connect(m_socket, qres->ai_addr, qres->ai_addrlen) == SOCKET_ERROR)
		{
			Close();
			continue;
		}
		return true;
	}

	return false;
#endif
}

YGAME_SERVER_END
