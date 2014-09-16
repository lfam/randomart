#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int bitwise(void);
void handcheck(int input);
long int str2hex(void);

int
main(int argc, char *argv[])
{
	str2hex();
	return 0;
}

long int
str2hex(void) {
	size_t len;
	char pair_of_chars[2];
	int i;
	long hexbyte;

	char input[1024];
	fgets(input, 1023, stdin);

	len = strlen(input);
	puts(input);
	printf("len of input is %zd\n\n",len);

	for (i = 0; i < len;i += 2) {
		memcpy(pair_of_chars,&input[i],2 * sizeof(char));
		printf("pair_of_chars is %s\n",pair_of_chars);
		hexbyte = strtol(pair_of_chars,NULL,16);
		printf("hexbyte is %ld\n",hexbyte);
	}

	return 0;
}

int
bitwise(void) {
	int i;
	char input[1024];
	fgets(input, 1023, stdin);
	size_t input_len = strlen(input);
	int iinput;
	int b;
	int x = 0, y = 0;

	for (i = 0; i < input_len; i++) {
		iinput = input[i];
		printf("%d\n",iinput);
		for (b = 0; b < 4; b++) {
			printf("%d two bit pair\n",b);
			printf("iinput is %d\n",iinput);
//			printf("iinput masked on 0x1 bit is %d\n",(iinput & 0x1));
	 		x += (iinput & 0x1) ? 1 : -1;

//			printf("iinput masked on 0x2 bit is %d\n",(iinput & 0x2));
			y += (iinput & 0x2) ? 1 : -1;

			printf("ternary results of iinput %d bit pair is %d %d\n\n",b,x,y);
			iinput = iinput >>2;
		}
	}

	return 0;
}

