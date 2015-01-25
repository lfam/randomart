#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "base64_d.h"

int
main(void)
{
	char triplet[4] = {0};
	char quad[5] = {0};
	
	int delim = 10; // ASCII newline
	int c;
	int i = 0;
	while (((c = getchar()) != delim) && (c != EOF)) {
		quad[i] = c;
		if (++i == 4) {
			base64_d(quad, triplet);
			printf("%s", triplet);
			i = 0;
			memset(quad, 0, 5);
			memset(triplet, 0, 4);
		}
	}

	if(i) {
		int j;
		for (j = i; j < 4; j++)	quad[j] = '=';
		base64_d(quad, triplet);
		printf("%s", triplet);
		memset(quad, 0, 5);
		memset(triplet, 0, 4);
	}
	return 0;
}
