YGAME_SERVER_BEGIN

// 用于监听连接的连接
class NetListener : public NetBase
{
public:
	NetListener();
	virtual ~NetListener();

	virtual bool Listen(int backlog = 5);
	virtual NetPConnect * Accept();
protected:
	virtual bool Bind();
};

YGAME_SERVER_END