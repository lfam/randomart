#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *mempcpy(void *dest, const void *src, size_t n);

void *
mempcpy(void *dest, const void *src, size_t n)
{
	return (char *)memcpy(dest, src, n) + n;
}

int main(int argc, char *argv[])
{
	char *destination = malloc(9);
	char *ptr = destination;

	if ( argc < 2 ) exit(1);

	int i;
	for (i = 1; i < argc; i++) {
		ptr = mempcpy (ptr, argv[i], strlen( argv[i] ));
	}

	puts(destination);
	free(destination);	
	free(ptr);
	exit(0);
}
