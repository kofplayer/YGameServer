YGAME_SERVER_BEGIN

// 通过listen生成的被动连接
class NetPConnect : public NetConnect
{
public:
	NetPConnect();
	virtual ~NetPConnect();

	virtual void SetSocket(SOCKET_ID s, NET_ADDR addr, NET_PORT port);
};

YGAME_SERVER_END