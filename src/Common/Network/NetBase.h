YGAME_SERVER_BEGIN

class NetBase
{
public:
	NetBase();
	virtual ~NetBase();
	virtual SOCKET_ID GetSocket();
	virtual NET_ADDR GetAddr();
	virtual NET_PORT GetPort();
	virtual void SetAddr(const NET_ADDR addr);
	virtual void SetPort(const NET_PORT port);
	virtual void Close();
protected:
	virtual bool Create(int type, bool isBlock);
	SOCKET_ID m_socket;
	NET_ADDR m_addr;
	NET_PORT m_port;
};

YGAME_SERVER_END