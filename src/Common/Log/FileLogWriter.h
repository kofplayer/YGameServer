YGAME_SERVER_BEGIN

class FileLogWriter : public LogWriter
{
public:
	FileLogWriter();
	virtual ~FileLogWriter();
	virtual void write(uint8 logLevel, const char * msg);
	void setFilePath(const char * path);
	void setFilePreName(const char * preName);
protected:
	bool needChangeFile(struct tm & t);
	void makeFileName(struct tm & t);
private:
	YString m_path;
	YString m_preName;
	FILE * m_file;
	int m_day;
	int m_hour;
};

YGAME_SERVER_END
