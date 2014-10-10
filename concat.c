#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *mempcpy(void *dest, const void *src, size_t n);
void *combine(void *o1, size_t s1, void *o2, size_t s2);

void *
combine (void *o1, size_t s1, void *o2, size_t s2)
{
	void *result = malloc (s1 + s2);
	if (result != NULL)
		mempcpy (mempcpy (result, o1, s1), o2, s2);
	return result;
}

void *
mempcpy(void *dest, const void *src, size_t n)
{
	return memcpy(dest, src, n) + n;
}

int main()
{
	char one[3]  = "ab";
	char two[3]  = "cd";
	char three[3]  = "ef";
	char four[3] = "gh";

	char *alphabet[] = {"ab", "cd", "de", "gh"};

	char *destination = malloc(9);
	char *ptr = destination;

	ptr = mempcpy(ptr, one, strlen(one)); 
	puts(destination);
	ptr = mempcpy(ptr, two, strlen(two));
	puts(destination);
/*
	puts("in for loop");
	puts("++++");
	for (int i = 0; i < 4; i++ ) {
		ptr = mempcpy(ptr,alphabet[i],strlen(alphabet[i]));
		puts(destination);
	}
	puts("----");
*/

	free(destination);	
}
