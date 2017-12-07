YGAME_SERVER_BEGIN

class DBResultMySql
{
public:
    DBResultMySql();
    virtual ~DBResultMySql();
    virtual bool isEnd();
    virtual void next();
    virtual const char * getFieldValue(const char * fieldName);
};

YGAME_SERVER_END
