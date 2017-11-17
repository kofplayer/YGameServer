#include "../Common.h"

YGAME_SERVER_BEGIN

LogWriter::LogWriter() : m_logLevel(0)
{

}

LogWriter::~LogWriter()
{

}

void LogWriter::setLogLevel(uint8 logLevel)
{
	m_logLevel = logLevel;
}

void LogWriter::write(uint8 logLevel, const char * msg)
{
	if (logLevel < m_logLevel)
	{
		return;
	}
	write(msg);
}

YGAME_SERVER_END