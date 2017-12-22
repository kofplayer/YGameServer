YGAME_SERVER_BEGIN

class DataBaseMySql : public DataBase
{
public:
    DataBaseMySql();
    virtual ~DataBaseMySql();
    virtual void setInfo(const char * host, uint16 port, const char * userName, const char * password, const char * dataBase);
    virtual bool query(const char * cmd, DBResult * result);
protected:
    bool connect();
    void disconnect();
    bool isConnect();
    
    YString m_host;
    uint16 m_port;
    YString m_userName;
    YString m_password;
    YString m_dataBase;
    
    MYSQL * m_dbHandle;
};

YGAME_SERVER_END
