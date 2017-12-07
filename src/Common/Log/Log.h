YGAME_SERVER_BEGIN

class Log
{
public:
    Log();
	virtual ~Log();
	void addLogWriter(LogWriter * logWriter);
    void removeLogWriter(LogWriter * logWriter);
    virtual void setLogLevel(uint8 logLevel);
    virtual bool isLogEnable(uint8 logLevel);
    virtual void writeLog(uint8 logLevel, const char * fileName, const int line, const char * msg, ...);
protected:
    int8 m_logLevel;
    MutexLock m_threadLock;
	List<LogWriter*> m_writerList;
};

#define LL_DEBUG 0
#define LL_INFO 1
#define LL_WARN 2
#define LL_ERROR 3
#define LL_FATAL 4

class GlobalLog : public Singleton<GlobalLog>, public Log
{
};

#define LOG_ADD_WRITER(writer)  GlobalLog::getInstance()->addLogWriter(writer);
#define LOG_REMOVE_WRITER(writer)  GlobalLog::getInstance()->removeLogWriter(writer);

#define SET_LOG_LEVEL(level) GlobalLog::getInstance()->setLogLevel(level)
#define LOG(level, ...) if(GlobalLog::getInstance()->isLogEnable(level))GlobalLog::getInstance()->writeLog(level, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) LOG(LL_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) LOG(LL_INFO, __VA_ARGS__)
#define LOG_WARN(...) LOG(LL_WARN, __VA_ARGS__)
#define LOG_ERROR(...) LOG(LL_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) LOG(LL_FATAL, __VA_ARGS__)

YGAME_SERVER_END
