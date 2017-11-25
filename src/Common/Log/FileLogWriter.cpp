#include "../Common.h"

YGAME_SERVER_BEGIN

#define  MAX_FILE_NAME_LEN	255

FileLogWriter::FileLogWriter() : m_file(NULL)
{
	m_path = "log";
	m_day = 0;
	m_hour = 0;
}

FileLogWriter::~FileLogWriter()
{
	fclose(m_file);
}

void FileLogWriter::write(uint8 logLevel, const char * msg)
{
	if ( access(m_path.c_str() , 0 ) == -1 )
	{
#if CURRENT_PLATFORM == PLATFORM_WIN32
		mkdir(m_path.c_str());
#else
		mkdir(m_path.c_str(), S_IRWXU);
#endif
	}

	time_t clock = time( NULL );
	struct tm t ;
#ifndef WIN32
	localtime_r(&clock ,&t);
#else
	localtime_s(&t ,&clock);
#endif
	if (needChangeFile(t))
	{
		makeFileName(t);
	}
	fwrite(msg, strlen(msg), 1, m_file);
	fflush(m_file);
}

void FileLogWriter::setFilePath(const char * path)
{
	m_path = path;
}

void FileLogWriter::setFilePreName(const char * preName)
{
	m_preName = preName;
}

bool FileLogWriter::needChangeFile(struct tm & t)
{
	return t.tm_mday != m_day || t.tm_hour != m_hour;
}

void FileLogWriter::makeFileName(struct tm & t)
{
	m_day = t.tm_mday ;
	m_hour = t.tm_hour;
	char fileName[MAX_FILE_NAME_LEN];
	snprintf( fileName, MAX_FILE_NAME_LEN, "%s/%s%04d%02d%02d-%02d.log", m_path.c_str(), m_preName.c_str(), t.tm_year + 1900, t.tm_mon + 1,t.tm_mday, t.tm_hour );

	if (m_file != NULL)
	{
		fclose(m_file);
		m_file = NULL;	
	}
	m_file = fopen(fileName,"a+");
}

YGAME_SERVER_END
