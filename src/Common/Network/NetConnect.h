YGAME_SERVER_BEGIN

class NetConnect : public NetBase
{
public:
	NetConnect();
	virtual ~NetConnect();

	virtual int32 Send(const void * data, int32 len);
	virtual int32 Recv(void * data, int32 len);
};

YGAME_SERVER_END
