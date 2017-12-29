YGAME_SERVER_BEGIN

class DataBaseMySql : public DataBase
{
public:
    DataBaseMySql();
    virtual ~DataBaseMySql();
    virtual void SetInfo(const char * host, uint16 port, const char * userName, const char * password, const char * dataBase);
    virtual bool Query(const char * cmd, DBResult * result);
protected:
    bool Connect();
    void Disconnect();
    bool IsConnect();
    
    YString m_host;
    uint16 m_port;
    YString m_userName;
    YString m_password;
    YString m_dataBase;
    
    MYSQL * m_dbHandle;
};

YGAME_SERVER_END
