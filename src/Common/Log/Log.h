YGAME_SERVER_BEGIN

class LogWriter
{
public:
	LogWriter() {}
	virtual ~LogWriter() {}
	virtual void write(const char * msg) = 0;
};

class Log : public Singleton<Log>
{
public:
    Log();
	virtual ~Log();
	void addLogWriter(LogWriter * logWriter);
	void removeLogWriter(LogWriter * logWriter);
	void setLogLevel(uint8 logLevel);
	void writeLog(uint8 logLevel, const char * fileName, const int line, const char * msg, ...);
protected:
	int8 m_logLevel;
	ThreadMutex m_threadMutex;
	List<LogWriter*> m_writerList;
};

#define SET_LOG_LEVEL(level)  Log::getInstance()->setLogLevel(level);
#define LOG_ADD_WRITER(writer)  Log::getInstance()->addLogWriter(writer);
#define LOG_REMOVE_WRITER(writer)  Log::getInstance()->removeLogWriter(writer);

#define LOG(level, ...) Log::getInstance()->writeLog(level, __FILE__, __LINE__, __VA_ARGS__ )
#define LOG_DEBUG(...) LOG(0, __VA_ARGS__)
#define LOG_ERROR(...) LOG(5, __VA_ARGS__)

YGAME_SERVER_END