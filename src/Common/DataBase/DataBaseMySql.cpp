﻿#include "../Common.h"

YGAME_SERVER_BEGIN

DataBaseMySql::DataBaseMySql() : m_port(0), m_dbHandle(NULL)
{
}

DataBaseMySql::~DataBaseMySql()
{
	Disconnect();
}

void DataBaseMySql::SetInfo(const char * host, uint16 port, const char * userName, const char * password, const char * dataBase)
{
    m_host = host;
    m_port = port;
    m_userName = userName;
    m_password = password;
    m_dataBase = dataBase;
}

bool DataBaseMySql::Query(const char * cmd, DBResult * result)
{
    if (!IsConnect()) {
        Connect();
    }
    if (!IsConnect()) {
        return false;
    }
    
    if (mysql_real_query(m_dbHandle, cmd, strlen(cmd)) != 0)
    {
        LOG_ERROR("errno=%d err=%s sql=%s\n", mysql_errno(m_dbHandle), mysql_error(m_dbHandle), cmd);
        if (mysql_errno(m_dbHandle) == 2006 || mysql_errno(m_dbHandle) == 2013)
        {
            if (!Connect())
            {
                return false;
            }
            if (mysql_real_query(m_dbHandle, cmd, strlen(cmd)) != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    
    MYSQL_RES * res = NULL;
    do
    {
        MYSQL_RES *  temp = mysql_store_result(m_dbHandle) ;
        if (temp == NULL)
        {
            break;
        }
		res = temp;
    } while (!mysql_next_result(m_dbHandle));
	dynamic_cast<DBResultMySql*>(result)->SetDataSet(res);
    return true;
}

bool DataBaseMySql::Connect()
{
    Disconnect();
    mysql_library_init(0,NULL,NULL);
    m_dbHandle = mysql_init((MYSQL*)NULL);
    if (m_dbHandle == NULL)
    {
        LOG_ERROR("database m_dbHandle == NULL errno = %d err = %s ", mysql_errno(m_dbHandle), mysql_error(m_dbHandle));
        return false ;
    }
    int timeout = 60 ;
    mysql_options( m_dbHandle ,MYSQL_OPT_CONNECT_TIMEOUT , (const void *)&timeout );
    mysql_options( m_dbHandle ,MYSQL_OPT_READ_TIMEOUT , (const void *)&timeout );
    mysql_options( m_dbHandle ,MYSQL_OPT_WRITE_TIMEOUT , (const void *)&timeout );
    if ( mysql_real_connect( m_dbHandle, m_host.c_str(), m_userName.c_str(), m_password.c_str(), m_dataBase.c_str(), m_port, NULL, CLIENT_BASIC_FLAGS & ~CLIENT_NO_SCHEMA & ~CLIENT_IGNORE_SIGPIPE/*clientOpt | CLIENT_INTERACTIVE*/ ) == NULL )
    {
        LOG_ERROR("mysql_real_connect errno = %d err = %s\n", mysql_errno(m_dbHandle), mysql_error(m_dbHandle));
        Disconnect();
        return false ;
    }
    mysql_set_character_set(m_dbHandle, "utf8");
    
    return true ;
}

void DataBaseMySql::Disconnect()
{
    if ( m_dbHandle != NULL )
    {
        mysql_close(m_dbHandle);
        m_dbHandle = NULL;
        mysql_library_end();
    }
}

bool DataBaseMySql::IsConnect()
{
    return m_dbHandle != NULL;
}

YGAME_SERVER_END
