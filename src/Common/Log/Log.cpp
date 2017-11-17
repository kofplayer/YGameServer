#include "../Common.h"

YGAME_SERVER_BEGIN

Log::Log()
{
}

Log::~Log()
{
    
}

void Log::addLogWriter(LogWriter * logWriter)
{
	AutoThreadMutex autoMutex(&m_threadMutex);
	m_writerList.push_back(logWriter);
}

void Log::removeLogWriter(LogWriter * logWriter)
{
	AutoThreadMutex autoMutex(&m_threadMutex);
	for (auto itor = m_writerList.begin(); itor != m_writerList.end(); ++itor)
	{
		if (*itor == logWriter)
		{
			m_writerList.erase(itor);
			break;
		}
	}
}

void Log::writeLog(uint8 logLevel, const char * fileName, const int line, const char * msg, ...)
{
	AutoThreadMutex autoMutex(&m_threadMutex);
	if (logLevel == 255 || 0 == m_writerList.size())
	{
		return;
	}

	time_t clock = time( NULL );
	struct tm t ;
#ifndef WIN32
	localtime_r(&clock ,&t);
#else
	localtime_s(&t ,&clock);
#endif
	static char tempBuffer[8192];
	snprintf(tempBuffer, sizeof(tempBuffer), "[%04d-%02d-%02d %02d:%02d:%02d][%s:%d][level:%u]: ",
		t.tm_year + 1900, t.tm_mon + 1,
		t.tm_mday, t.tm_hour,  t.tm_min,  t.tm_sec ,
		fileName, line ,logLevel);

	char * p = tempBuffer + strlen(tempBuffer);
	size_t leftSize = sizeof(tempBuffer) - strlen(tempBuffer);

	va_list ap;
	va_start(ap, msg);
	vsnprintf(p, leftSize-1, msg, ap);
	va_end(ap);
	p[leftSize-1] = '\0';

	for (auto itor = m_writerList.begin(); itor != m_writerList.end(); ++itor)
	{
		(*itor)->write(logLevel, tempBuffer);
	}
}

YGAME_SERVER_END