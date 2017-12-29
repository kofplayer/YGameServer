YGAME_SERVER_BEGIN

class DBResult
{
public:
    DBResult() {}
    virtual ~DBResult() {}
    virtual bool IsEnd() = 0;
	virtual void Next() = 0;
	virtual void MoveToFirst() = 0;
	virtual void MoveToLast() = 0;
	virtual uint32 GetRowCount() = 0;
	virtual uint32 GetFieldCount() = 0;
	virtual const char * GetFieldValue(const char * fieldName) = 0;
	virtual int64 GetFieldInt(const char * fieldName);
	virtual float GetFieldFloat(const char * fieldName);
};

YGAME_SERVER_END
