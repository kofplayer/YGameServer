YGAME_SERVER_BEGIN

// 用于监听连接的连接
class NetListener : public NetBase
{
public:
	NetListener();
	virtual ~NetListener();

	virtual bool Create(int type = SOCK_STREAM);
	virtual bool Bind(NET_ADDR addr, NET_PORT port);
	virtual bool Listen(int backlog = 5);
	virtual NetPConnect * Accept();
};

YGAME_SERVER_END
