YGAME_SERVER_BEGIN

class DataBase
{
public:
    DataBase();
    virtual ~DataBase();
    virtual bool setInfo(const char * address, uint16 port, const char * account, const char * password, const char * datebase) = 0;
    virtual bool query(const char * cmd, DBResult * result) = 0;
};

YGAME_SERVER_END
