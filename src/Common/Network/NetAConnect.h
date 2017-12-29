YGAME_SERVER_BEGIN

// 主动connect的连接
class NetAConnect : public NetConnect
{
public:
	NetAConnect();
	virtual ~NetAConnect();

	virtual bool Connect(bool isBlock);
	virtual bool ConnectUrl(const char * url);
};

YGAME_SERVER_END