YGAME_SERVER_BEGIN

class DBResultMySql : public DBResult
{
	friend class DataBaseMySql;
public:
	DBResultMySql();
	virtual ~DBResultMySql();
	virtual void moveToFirst();
	virtual void moveToLast();
	virtual uint32 getRowCount();
	virtual uint32 getFieldCount();
	virtual void next();
	virtual bool isEnd();
	virtual void clear();
	virtual const char * getFieldValue(const char * fieldName);
protected:
	void setDataSet(MYSQL_RES * query);
private:
	int32 getFieldIndex(const char * fieldName);
	void getDataSetInfo();
    MYSQL_RES *		m_dataSet;
    MYSQL_FIELD *   m_fieldInfo;
    MYSQL_ROW       m_row;
    unsigned long   m_rowCount;
    int             m_fieldCount;
};

YGAME_SERVER_END
