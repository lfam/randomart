#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "strtol_wrap.h"

int main(void)
{
	ssize_t	line_len;
	char	*line = NULL;
	size_t	line_buf_len = 0;

	while ((line_len = getdelim(&line, &line_buf_len, 10, stdin)) > 0) {
		if (line_len < 0) {
			perror("ERROR getdelim()");
			return 1;
		}
		line[line_len - 1] = '\0';
		
		char * errstring = NULL;
		if ((errstring = malloc(line_len + 1)) == NULL) {
			perror("ERROR malloc()");
			return 1;
		}
		errstring[line_len] = '\0';
		char *errptr = errstring;

		long parsed;
		if (!strtol_wrap(&parsed, line, 16, &errptr)) {
			fputs(errstring, stderr);
			fprintf(stderr, "<-- failed\n");
		}
		printf("%ld\n", parsed);
		free(errstring);
	}
	free(line);
	return 0;
}	

