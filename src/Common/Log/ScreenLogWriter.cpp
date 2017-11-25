#include "../Common.h"

YGAME_SERVER_BEGIN

void ScreenLogWriter::write(const char * msg)
{
	printf("%s", msg);
}

YGAME_SERVER_END
