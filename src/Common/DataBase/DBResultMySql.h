YGAME_SERVER_BEGIN

class DBResultMySql : public DBResult
{
	friend class DataBaseMySql;
public:
	DBResultMySql();
	virtual ~DBResultMySql();
	virtual void MoveToFirst();
	virtual void MoveToLast();
	virtual uint32 GetRowCount();
	virtual uint32 GetFieldCount();
	virtual void Next();
	virtual bool IsEnd();
	virtual void Clear();
	virtual const char * GetFieldValue(const char * fieldName);
protected:
	void SetDataSet(MYSQL_RES * query);
private:
	int32 GetFieldIndex(const char * fieldName);
	void GetDataSetInfo();
    MYSQL_RES *		m_dataSet;
    MYSQL_FIELD *   m_fieldInfo;
    MYSQL_ROW       m_row;
    unsigned long   m_rowCount;
    int             m_fieldCount;
};

YGAME_SERVER_END
