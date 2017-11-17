YGAME_SERVER_BEGIN

class Log : public Singleton<Log>
{
public:
    Log();
	virtual ~Log();
	void addLogWriter(LogWriter * logWriter);
	void removeLogWriter(LogWriter * logWriter);
	void writeLog(uint8 logLevel, const char * fileName, const int line, const char * msg, ...);
protected:
	ThreadMutex m_threadMutex;
	List<LogWriter*> m_writerList;
};

#define LL_DEBUG 0
#define LL_INFO 1
#define LL_WARN 2
#define LL_ERROR 3
#define LL_FATAL 4

#define LOG_ADD_WRITER(writer)  Log::getInstance()->addLogWriter(writer);
#define LOG_REMOVE_WRITER(writer)  Log::getInstance()->removeLogWriter(writer);

#define LOG(level, ...) Log::getInstance()->writeLog(level, __FILE__, __LINE__, __VA_ARGS__ )
#define LOG_DEBUG(...) LOG(LL_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) LOG(LL_INFO, __VA_ARGS__)
#define LOG_WARN(...) LOG(LL_WARN, __VA_ARGS__)
#define LOG_ERROR(...) LOG(LL_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) LOG(LL_FATAL, __VA_ARGS__)

YGAME_SERVER_END