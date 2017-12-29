#include "../Common.h"

YGAME_SERVER_BEGIN

int64 DBResult::GetFieldInt(const char * fieldName)
{
	const char * value = GetFieldValue(fieldName);
	if (value == NULL) return 0;
	return atol(value);
}

float DBResult::GetFieldFloat(const char * fieldName)
{
	const char * value = GetFieldValue(fieldName);
	if (value == NULL) return 0;
	return (float)atof(value);
}

YGAME_SERVER_END
