#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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


int decode(char *quad, char *out);

int
decode(char *quad, char *out)
{
	/* base64 encoding array, for reference */
	//const char encode[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

	int i = 0, j;
	char buff1[4];
	char buff2[4];

	while ((buff2[i] = quad[i]) && (buff2[i] != '=')) {
//		fprintf(stderr,
//			"buff2[%d] is %d\n", i, buff2[i]);
		if ( ++i == 4) {

			for (i = 0; i != 4; i++) 
				buff2[i] = alphabet[(int)buff2[i]];
			
			out[0] = ((buff2[0] << 2) ^ ((buff2[1] & 0x30) >> 4));  
			out[1] = (((buff2[1] & 0x0f) << 4) ^ ((buff2[2] & 0x3c) >> 2));
			out[2] = (((buff2[2] & 0x03) << 6) ^ (buff2[3] & 0x3f));

			i = 0;
			return 1;
		}
	}

	if (i) {
		fprintf(stderr, "in padding block as i is %d\n", i);
		for (j = i; j < 4; j++) {
			buff2[j] = '\0';
//			printf("buff2[%d] is %d\n", j, buff2[j]);
		}
		for (j = 0; j < 4; j++) {
			fprintf(stderr,
				"buff2[%d] raw is %d\n", j, buff2[j]);
			if (buff2[j] != 0)
				buff2[j] = alphabet[(int)buff2[j]];
			printf("buff2[%d] decoded to %d\n", j, buff2[j]);
		}

		out[0] = ((buff2[0] << 2) ^ ((buff2[1] & 0x30) >> 4));  
		fprintf(stderr,
			"out[0] is %c\n", out[0]);
		out[1] = (((buff2[1] & 0x0f) << 4) ^ ((buff2[2] & 0x3c) >> 2));
//		fprintf(stderr, "buff1[2] is uninitialized\n");
		out[2] = (((buff2[2] & 0x03) << 6) ^ (buff2[3] & 0x3f));
//		fprintf(stderr, "now, buff1[2] is %c\n", buff1[2]);
/*		
		for (j = 0; j < (i - 1); j++)
			fprintf(stderr, "j = %d\ni = %d\n", j, i);
			fprintf(stderr,
				"buff1[%d] is %c\n", j, buff1[j]);
			out[j] = buff1[j];
			fprintf(stderr,
				"new out[%d] is %c\n", j, out[j]);
*/
	}
	return 1;
}




int
main(void)
{
	char 	*line = NULL;
	size_t	buf_len = 0;
	ssize_t	len;
	int	delim = 10; // ASCII newline

	char	quad[5];
	quad[4] = '\0';
	char 	out[4];
	out[3] = '\0';

	while((len = getdelim(&line, &buf_len, delim, stdin)) > 0) {
		line[len - 1] = '\0';
		int i;
		for (i = 0; i < len - 1; i += 4) {
			quad[0] = line[0 + i];
			quad[1] = line[1 + i];
			quad[2] = line[2 + i];
			quad[3] = line[3 + i];
			printf("base64: %s\n", quad);
			decode(quad, out);
			printf("english: %s\n", out);
			printf("\n");
		}
	}
//		printf("\n");
	

	free(line);
	return 0;
}
