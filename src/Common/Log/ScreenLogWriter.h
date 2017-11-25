YGAME_SERVER_BEGIN

class ScreenLogWriter : public LogWriter
{
public:
	virtual void write(uint8 logLevel, const char * msg);
};

YGAME_SERVER_END
