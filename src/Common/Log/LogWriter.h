YGAME_SERVER_BEGIN

class LogWriter
{
public:
	LogWriter();
	virtual ~LogWriter();
	virtual void write(uint8 logLevel, const char * msg) = 0;
};

YGAME_SERVER_END
