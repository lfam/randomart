#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
	(byte & 0x80 ? 1 : 0), \
	(byte & 0x40 ? 1 : 0), \
	(byte & 0x20 ? 1 : 0), \
	(byte & 0x10 ? 1 : 0), \
	(byte & 0x08 ? 1 : 0), \
	(byte & 0x04 ? 1 : 0), \
	(byte & 0x02 ? 1 : 0), \
	(byte & 0x01 ? 1 : 0) 


#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	ssize_t	line_len;
	char	*line = NULL;
	size_t	line_buf_len = 0;

	while ((line_len = getdelim(&line, &line_buf_len, 10, stdin)) > 0) {
		if (line_len < 0) {
			perror("ERROR getdelim");
			return 1;
		}

		int byte = (int)line[0];
		printf(BYTETOBINARYPATTERN, BYTETOBINARY(byte));
		printf("\n");
	}
	free(line);
	return 0;
}


