#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	unsigned long oct;
	char	*end;
	char	*str;

	if ( argc > 1 ) {
		str = argv[1];
	} else {
		fprintf(stderr,"You need to enter an argument\n");
		exit(1);
	}

	oct = strtoul(str, &end, 8);

	if (end == str) {
		 fprintf(stderr,"%s: not a octal number\n",
			 str);
		 exit(1);
	} else if ('\0' != *end) {
		fprintf(stderr,"%s: extra characters at end of input: %s\n", str, end);
		exit(1);
	}
	printf("oct is %lu\n", oct);

	return 0;
}
