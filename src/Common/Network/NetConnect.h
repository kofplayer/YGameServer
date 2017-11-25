YGAME_SERVER_BEGIN

class NetConnect : public NetBase
{
public:
	NetConnect();
	virtual ~NetConnect();

	virtual int64 Send(const void * data, int32 len);
	virtual int64 Recv(void * data, int32 len);
};

YGAME_SERVER_END
