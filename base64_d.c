#include <stdlib.h>
#include <stdio.h>

char *base64_d(char *quad);

char
*base64_d(char *quad)
{
	/* base64 decoding array  */
	const char alphabet[256] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
	52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,
	28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; 

	static char out[4];
	out[3] = '\0';

	int i = 0, j;
	char buff[4];

	while ((buff[i] = quad[i]) && (buff[i] != '=') && (buff[i] != '0')) {
		if ( ++i == 4) {
			for (i = 0; i != 4; i++) 
				buff[i] = alphabet[(int)buff[i]];
			
			out[0] = ((buff[0] << 2) ^ ((buff[1] & 0x30) >> 4));  
			out[1] = (((buff[1] & 0x0f) << 4) ^ ((buff[2] & 0x3c) >> 2));
			out[2] = (((buff[2] & 0x03) << 6) ^ (buff[3] & 0x3f));

			i = 0;
			return out;
		}
	}

	if (i) {
	
		for (j = 0; j < i; j++)
			buff[j] = alphabet[(int)buff[j]];
		/*
		 * The above and below blocks have been reversed. This way
		 * doesn't try to decode null bytes. The caller will print extra
		 * nulls so maybe that can get fixed.
		 */
		for (j = i; j < 4; j++)
			buff[j] = '\0';

		out[0] = ((buff[0] << 2) ^ ((buff[1] & 0x30) >> 4));  
		out[1] = (((buff[1] & 0x0f) << 4) ^ ((buff[2] & 0x3c) >> 2));
		out[2] = (((buff[2] & 0x03) << 6) ^ (buff[3] & 0x3f));

		return out;
	}
	return out;
	errno = 1;
}




int
main(void)
{
	int	delim = 10; // ASCII newline

	char	*quad = NULL;
	if ((quad = calloc(1, 5)) == NULL) return 1;
	
	int c;
	int i = 0;
	while (((c = getchar()) != delim) && (c != EOF)) {
		quad[i] = c;
		if (++i == 4) {
			printf("%s", base64_d(quad));
		/*	if ( !base64_d(quad, triplet)) {
				fprintf(stderr, "decoder error\n");
				return 1;
			}
		*/
			i = 0;
		}
	}

	if(i) {
		int j;
		for (j = i; j < 4; j++) {
			quad[j] = '=';
		}
		printf("%s", base64_d(quad));
	}
	printf("%c", delim);

	free(quad);
	return 0;
}
