#include "../Common.h"

YGAME_SERVER_BEGIN

DBResultMySql::DBResultMySql() : m_dataSet(NULL),
m_fieldCount(0),
m_row(NULL),
m_rowCount(0),
m_fieldInfo(NULL)
{
}

DBResultMySql::~DBResultMySql()
{
	clear();
}

void DBResultMySql::moveToFirst()
{
	if (m_dataSet != NULL)
	{
		mysql_data_seek(m_dataSet, 0);
	}
};

void DBResultMySql::moveToLast()
{
	if (m_dataSet != NULL)
	{
		mysql_data_seek(m_dataSet, m_rowCount - 1);
	}
}

uint32 DBResultMySql::getRowCount()
{
	return m_rowCount;
}

uint32 DBResultMySql::getFieldCount()
{
	return m_fieldCount;
}

void DBResultMySql::next()
{
	if (NULL == m_dataSet)
	{
		return;
	}
	m_row = mysql_fetch_row(m_dataSet);
}

bool DBResultMySql::isEnd()
{
	return m_row == NULL;
}

void DBResultMySql::clear()
{
	if (m_dataSet != NULL)
	{
		mysql_free_result(m_dataSet);
		m_dataSet = NULL;
	}
	m_fieldCount = 0;
	m_fieldInfo = NULL;
	m_row = NULL;
	m_rowCount = 0;
}

const char * DBResultMySql::getFieldValue(const char * fieldName)
{
	if (NULL == m_dataSet || fieldName == NULL || m_fieldInfo == NULL)
		return NULL;
	int fieldIndex = getFieldIndex(fieldName);
	if (fieldIndex >= 0 && fieldIndex < m_fieldCount)
	{
		if (m_row[fieldIndex] == NULL)
		{
			return "";
		}
		return m_row[fieldIndex];
	}
	return NULL;
}

int32 DBResultMySql::getFieldIndex(const char * fieldName)
{
	if (NULL == m_dataSet || fieldName == NULL || m_fieldInfo == NULL)
		return -1;

	for (int i = 0; i < m_fieldCount; i++)
	{
		// printf("Field %u is %s\n", i, m_fieldInfo [ i ].name);
		if (strcmp(fieldName, m_fieldInfo[i].name) == 0)
		{
			return i;
		}
	}
	char buffer[50];
	sprintf(buffer, "field :%s not found\n", fieldName);
	printf(buffer);
	throw buffer;
	return -1;
}

void DBResultMySql::setDataSet(MYSQL_RES * query)
{
	m_fieldCount = 0;
	m_fieldInfo = NULL;
	m_row = NULL;
	m_rowCount = 0;
	this->m_dataSet = query;
	getDataSetInfo();
}

void DBResultMySql::getDataSetInfo()
{
	if (m_dataSet != NULL)
	{
		mysql_data_seek(m_dataSet, 0);
		m_row = mysql_fetch_row(m_dataSet);
		m_rowCount = mysql_num_rows(m_dataSet);
		m_fieldCount = mysql_num_fields(m_dataSet);
		m_fieldInfo = mysql_fetch_fields(m_dataSet);
	}
}

YGAME_SERVER_END
