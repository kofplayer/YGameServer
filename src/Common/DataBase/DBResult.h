YGAME_SERVER_BEGIN

class DBResult
{
public:
    DBResult() {}
    virtual ~DBResult() {}
    virtual bool isEnd() = 0;
	virtual void next() = 0;
	virtual void moveToFirst() = 0;
	virtual void moveToLast() = 0;
	virtual uint32 getRowCount() = 0;
	virtual uint32 getFieldCount() = 0;
	virtual const char * getFieldValue(const char * fieldName) = 0;
	virtual int64 getFieldInt(const char * fieldName);
	virtual float getFieldFloat(const char * fieldName);
};

YGAME_SERVER_END
