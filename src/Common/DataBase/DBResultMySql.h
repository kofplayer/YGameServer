YGAME_SERVER_BEGIN

class DBResultMySql
{
public:
    DBResultMySql() : m_dataSet(NULL),
    m_fieldCount( 0 ),
    m_row(NULL),
    m_rowCount(0),
    m_fieldInfo(NULL)
    {
    }
    virtual ~DBResultMySql()
    {
        Finalize();
    }
    void setDataSet(MYSQL_RES * query)
    {
        m_fieldCount = 0 ;
        m_fieldInfo  = NULL ;
        m_row		 = NULL ;
        m_rowCount	 = 0 ;
        this->m_dataSet			= query;
        GetDataSetInfo();
        query = NULL ;
    }
    void moveToFirst()
    {
        if ( m_dataSet != NULL )
        {
            mysql_data_seek( m_dataSet , 0 );
        }
    };
    
    void moveToLast()
    {
        if ( m_dataSet != NULL )
        {
            mysql_data_seek( m_dataSet , m_rowCount - 1 );
        }
    }
    
    unsigned long getRowCount() { return m_rowCount;}
    
    int getFieldCount() {return m_fieldCount;}
    
    virtual void next();
    
    //ÊÇ·ñµ½ÁËÐÐÎ²
    virtual bool isEnd() { return m_row == NULL ;}
    
    //»ñÈ¡×Ö¶ÎµÄÕûÐÎÖµ
    long			GetFieldInt( const char * fieldName )
    {
        const char * value = GetFieldValue( fieldName) ;
        if ( value == NULL ) return 0 ;
        return atol(  value ) ;
    }
    
    
    //»ñÈ¡×Ö¶ÎµÄÐ¡ÊýÖµ
    float			GetFieldFloat( const char * fieldName )
    {
        const char * value = GetFieldValue( fieldName) ;
        if ( value == NULL ) return 0 ;
        return (float)atof(  value ) ;
        
    }
    
    virtual const char * getFieldValue(const char * fieldName)
    {
        if ( NULL == m_dataSet || fieldName == NULL || m_fieldInfo == NULL )
            return NULL ;
        int fieldIndex = GetFieldIndex( fieldName );
        if ( fieldIndex >= 0 && fieldIndex < m_fieldCount )
        {
            if ( m_row[ fieldIndex ] == NULL )
            {
                return "";
            }
            return m_row[ fieldIndex ] ;
        }
        return NULL;
    }
    
    //»ñÈ¡×Ö¶ÎµÄÖµ
    const char *	GetFieldValue( int fieldIndex )
    {
        if ( NULL == m_dataSet  )
            return NULL ;
        if ( fieldIndex >= 0 && fieldIndex < m_fieldCount )
        {
            
            return m_row[ fieldIndex ] ;
        }
        return NULL;
    }
    
    //ÊÍ·ÅÖ¸Õë
    void			Finalize() {
        if ( m_dataSet != NULL )
        {
            mysql_free_result( m_dataSet ) ;
            m_dataSet = NULL ;
        }
        
        m_fieldCount = 0 ;
        m_fieldInfo  = NULL ;
        m_row		 = NULL ;
        m_rowCount	 = 0 ;
    }
    MYSQL_FIELD * get_field_info(){
        return m_fieldInfo ;
    }
    bool is_primary_key( const int index ){
        return  PRI_KEY_FLAG & m_fieldInfo [ index ].flags;
        
    }
    int get_field_value_length( const int index ){
        return (int)m_fieldInfo [ index ].max_length ;
    }
private:
    MYSQL_RES *		m_dataSet;
    MYSQL_FIELD *   m_fieldInfo;
    MYSQL_ROW       m_row;
    unsigned long   m_rowCount;
    int             m_fieldCount;
};

YGAME_SERVER_END
