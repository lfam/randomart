#include <stdio.h>
#include <string.h>

int bitwise(void);

int
main(int argc, char *argv[])
{
	bitwise();
	return 0;
}

int
bitwise(void) {
	int i;
	char *input = "s,dmfnw,5mn 3m24n52 m,3n4,m23n4m,3 n,sdfefmn3 3mn mn3 mn3 m N#M N#M rn3m3nrmn mndmand mnma dld aowd iaw od uiawy duawy dhga hdabn bd3 8u9D#D@#DDWADBASDBSNAFBFAOFIUAWIOFJLKFmI";	
	size_t input_len = strlen(input);
	int iinput;
	int b;
	int x = 0, y = 0;

	for (i = 0; i < input_len; i++) {
		iinput = input[i];
		printf("%d\n",iinput);
		for (b = 0; b < 4; b++) {
			printf("%d two bit pair\n",b);

			printf("iinput masked on 0x1 bit is %d\n",(iinput & 0x1));
	 		x += (iinput & 0x1) ? 1 : -1;

			printf("iinput masked on 0x2 bit is %d\n",(iinput & 0x2));
			y += (iinput & 0x2) ? 1 : -1;

			printf("ternary results of iinput %d bit pair is %d %d\n\n",b,x,y);
			iinput = iinput >>2;
		}
	}

	return 0;
}
