YGAME_SERVER_BEGIN

class DBResult
{
public:
    DBResult() {}
    virtual ~DBResult() {}
    virtual bool isEnd() = 0;
    virtual void next() = 0;
    virtual const char * getFieldValue(const char * fieldName) = 0;
};

YGAME_SERVER_END
