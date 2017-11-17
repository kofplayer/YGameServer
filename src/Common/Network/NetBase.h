YGAME_SERVER_BEGIN

class NetBase
{
public:
	NetBase();
	virtual ~NetBase();
	virtual NET_ADDR GetAddr();
	virtual NET_PORT GetPort();
protected:
	virtual bool _create(int type);
	SOCKET_ID m_socket;
	NET_ADDR m_addr;
	NET_PORT m_port;
};

YGAME_SERVER_END