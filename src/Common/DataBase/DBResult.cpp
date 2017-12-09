#include "../Common.h"

YGAME_SERVER_BEGIN

int64 DBResult::getFieldInt(const char * fieldName)
{
	const char * value = getFieldValue(fieldName);
	if (value == NULL) return 0;
	return atol(value);
}

float DBResult::getFieldFloat(const char * fieldName)
{
	const char * value = getFieldValue(fieldName);
	if (value == NULL) return 0;
	return (float)atof(value);
}

YGAME_SERVER_END
