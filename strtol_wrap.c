#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int
strtol_wrap(const char *str, long *val, int radix, char **errptr)
{
	char *end;
	int ret = 1;

	int saved = errno;
	errno = 0;
	*val = strtol(str, &end, radix);

	if (*end != '\0' || end == str ||
		(((long)val == LONG_MIN || (long)val == LONG_MAX)
		&& errno == ERANGE)) {
		if (errptr != NULL ) memcpy(*errptr, str, strlen(str));
		ret = 0;
	}
	if (errno == 0) errno = saved;
	return ret;
}
