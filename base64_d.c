#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int base64_d(char *quad, char *out);

int
base64_d(char *quad, char *out)
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
			return 1;
		}
	}

	if (i) {
	
		for (j = 0; j < i; j++)
			buff[j] = alphabet[(int)buff[j]];
		/*
		 * The above and below blocks used to be reversed. This way
		 * doesn't try to decode null bytes. The caller will print extra
		 * nulls so maybe that can get fixed.
		 */
		for (j = i; j < 4; j++)
			buff[j] = '\0';

		out[0] = ((buff[0] << 2) ^ ((buff[1] & 0x30) >> 4));  
		out[1] = (((buff[1] & 0x0f) << 4) ^ ((buff[2] & 0x3c) >> 2));
		out[2] = (((buff[2] & 0x03) << 6) ^ (buff[3] & 0x3f));

	/*
	 * This was in the c++ version. It means that you don't emit the
	 * padded characters at all -- not nulls. 
	 *
	 *	for (j=0;j<(i-1); j++) out << (char)buff1[j];
	 */
		return 1;
	}
	return 0;
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
	char 	triplet[4];
	triplet[3] = '\0';

	while((len = getdelim(&line, &buf_len, delim, stdin)) > 0) {
		line[len - 1] = '\0';
		int i;
		for (i = 0; i < len - 1; i += 4) {
			quad[0] = line[0 + i];
			quad[1] = line[1 + i];
			quad[2] = line[2 + i];
			quad[3] = line[3 + i];
			if ( !base64_d(quad, triplet)) return 1;
			fputs(triplet, stdout);
		}
		printf("%c", delim);
	}
	

	free(line);
	return 0;
}
