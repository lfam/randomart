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

	for (int i = 1; i < argc; i++) {
		ptr = mempcpy (ptr, argv[i], strlen( argv[i] ));
	}

	puts(destination);
	free(destination);	
}
