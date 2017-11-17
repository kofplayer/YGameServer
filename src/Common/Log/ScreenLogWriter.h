YGAME_SERVER_BEGIN

class ScreenLogWriter : public LogWriter
{
public:
	virtual void write(const char * msg);
};

YGAME_SERVER_END