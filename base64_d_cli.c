#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "base64_d.h"

int
main(void)
{
	char *quad = NULL;
	if ((quad = calloc(1, 5)) == NULL) return 1;

	char trpl[4] = {'\0'};
	
	int delim = 10; // ASCII newline
	int c;
	int i = 0;
	while (((c = getchar()) != delim) && (c != EOF)) {
		quad[i] = c;
		if (++i == 4) {
			if (!base64_d(quad, trpl)) return 1; 
			puts(trpl);
			i = 0;
			memset(quad, '\0', 5);
		}
	}

	if(i) {
		int j;
		for (j = i; j < 4; j++)	quad[j] = '=';
		if (!base64_d(quad, trpl)) return 1; 
		puts(trpl);
		memset(quad, '\0', 5);
	}

	free(quad);
	return 0;
}
