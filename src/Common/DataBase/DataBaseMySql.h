YGAME_SERVER_BEGIN

class DataBaseMySql
{
public:
    DataBaseMySql();
    virtual ~DataBaseMySql();
    virtual bool setInfo(const char * address, uint16 port, const char * account, const char * password, const char * datebase);
    virtual bool query(const char * cmd, DBResult * result);
protected:
    bool connect();
    void disconnect();
    bool isConnect();
};

YGAME_SERVER_END
