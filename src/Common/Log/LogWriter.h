YGAME_SERVER_BEGIN

class LogWriter
{
public:
	LogWriter();
	virtual ~LogWriter();
	virtual void setLogLevel(uint8 logLevel);
	virtual void write(uint8 logLevel, const char * msg);
	virtual void write(const char * msg) = 0;
protected:
	int8 m_logLevel;
};

YGAME_SERVER_END