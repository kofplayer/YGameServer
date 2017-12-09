YGAME_SERVER_BEGIN

class DataBase
{
public:
	DataBase() {}
	virtual ~DataBase() {}
    virtual void setInfo(const char * host, uint16 port, const char * userName, const char * password, const char * dataBase) = 0;
    virtual bool query(const char * cmd, DBResult * result) = 0;
};

YGAME_SERVER_END
