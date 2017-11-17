YGAME_SERVER_BEGIN

// 主动connect的连接
class NetAConnect : public NetConnect
{
public:
	NetAConnect();
	virtual ~NetAConnect();

	virtual bool Create(int type = SOCK_STREAM);

protected:
	NET_ADDR m_addr;
	NET_PORT m_port;
};

YGAME_SERVER_END