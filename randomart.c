#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

/* Error codes:
 * 0	okay
 * 1	failure
 * 2	not all input characters could be processed
 */

/*
 * Draw an ASCII-Art representing the fingerprint so human brain can
 * profit from its built-in pattern recognition ability.
 * This technique is called "random art" and can be found in some
 * scientific publications like this original paper:
 *
 * "Hash Visualization: a New Technique to improve Real-World Security",
 * Perrig A. and Song D., 1999, International Workshop on Cryptographic
 * Techniques and E-Commerce (CrypTEC '99)
 * sparrow.ece.cmu.edu/~adrian/projects/validation/validation.pdf
 *
 * The subject came up in a talk by Dan Kaminsky, too.
 *
 * If you see the picture is different, the key is different.
 * If the picture looks the same, you still know nothing.
 *
 * The algorithm used here is a worm crawling over a discrete plane,
 * leaving a trace (augmenting the field) everywhere it goes.
 * Movement is taken from userstr 2bit-wise.  Bumping into walls
 * makes the respective movement vector be ignored for this turn.
 * Graphs are not unambiguous, because circles in graphs can be
 * walked in either direction.
 */

/* global variables */
static int error = 0;

/* function prototypes */
char *fingerprint_randomart(char *userstr, size_t userstr_len, size_t usr_fldbase);
long strtol_wrapper(char *num_str, char **errptr, int base);
int is_whitespace(const char *s);

/* functions */
/* is_whitespace() adapted from:
   http://stackoverflow.com/a/3981593
*/
int
is_whitespace(const char *s)
{
	while (*s != '\0') {
		if (!isspace(*s))
			return 0;
			s++;
	}
	return 1;
}

/*
 * adapted from:
 * http://rus.har.mn/blog/2014-05-19/strtol-error-checking/
 */
long 
strtol_wrapper(char *num_str, char **errptr, int base)
{
	size_t	num_strlen = strlen(num_str);
	char	*end = NULL;
	errno 	= 0;
	long	ret = strtol(num_str, &end, base);
	if ((LONG_MIN == ret || LONG_MAX == ret) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type long\n", num_str);
		return -1;
	} else if (errno != 0) {
		fprintf(stderr, "strtol error!\n");
		return -1;
	} else if ( '\0' != *end ) {
		if (errptr != NULL) {
		memcpy(*errptr, num_str, num_strlen);
		*errptr += num_strlen;
		}
		return -1;
	} else {
		if (errptr != NULL) {
		memset(*errptr, ' ', num_strlen);
		*errptr += num_strlen;
		}
		return ret;
	}
}

char * 
fingerprint_randomart(char *userstr, size_t userstr_len, size_t usr_fldbase)
{
	/*
	 * Chars to be used after each other every time the worm
	 * intersects with itself.  Matter of taste.
	 */
	char	*augmentation_string = " .o+=*BOX@%&#/^SE";
	char	*retval, *p;
	size_t	b;
	ssize_t	x, y;
	size_t	len = strlen(augmentation_string) - 1;
	size_t fld_x, fld_y;
	size_t	i;

	/*
	* Field sizes for the random art.  Have to be odd, so the starting point
	* can be in the exact middle of the picture, and FLDBASE should be >=8 .
	* Else pictures would be too dense, and drawing the frame would
	* fail, too, because the key type would not fit in anymore.
	*/
	fld_x = usr_fldbase * 2 + 1;
	fld_y = usr_fldbase + 1;

	unsigned char	field[fld_x][fld_y];
	
	if ((retval = calloc(((size_t)fld_x + 3), ((size_t)fld_y + 2))) == NULL) {
		fprintf(stderr, "ERROR: failed to allocate array.\n");
		return NULL;
	}

	/* initialize field */
	memset(field, 0, fld_x * fld_y * sizeof(char));
	x = fld_x / 2;
	y = fld_y / 2;

	/* set up error reporting for strtol() on user's input */
	char	*errstring = NULL;
	if ((errstring = malloc(userstr_len + 1)) == NULL) {
		fprintf(stderr, "ERROR: failed to allocate memory.\n");
		return NULL;
	}
	errstring[userstr_len] = '\0';
	char 	*errptr = errstring;

	/* process user's input */
	for (i = 0; i < userstr_len; i+=2) {

		/* break off two characters (i.e. one hex byte) */
		char	num_str[3];
		memset(num_str, 0, sizeof(num_str));
		memcpy(num_str, &userstr[i], sizeof(num_str) - 1);
		
		unsigned char	byte = '\0';
		long		strtol_ret = 0;
		if ((strtol_ret = strtol_wrapper(num_str, &errptr, 16)) < 0 ) {
			error = 2;
			continue;
		} else {
			byte = (unsigned char)strtol_ret;
		}


		/* each byte conveys four 2-bit move commands */
		for (b = 0; b < 4; b++) {
			/* evaluate 2 bit, rest is shifted later */
			x += (byte & 0x1) ? 1 : -1;
			y += (byte & 0x2) ? 1 : -1;

			/* assure we are still in bounds */
			x = MAX(x, 0);
			y = MAX(y, 0);
			x = MIN(x, (ssize_t)fld_x - 1);
			y = MIN(y, (ssize_t)fld_y - 1);

			/* augment the field */
			if (field[x][y] < len - 2)
				field[x][y]++;
			byte = byte >> 2;
		}
	}

	if ( !is_whitespace(errstring)) {
		fputs(errstring, stderr);
		fprintf(stderr, "<-- ERROR: not hexadecimal\n");
	}
	free(errstring);

	/* mark starting point and end point*/
	field[fld_x / 2][fld_y / 2] = len - 1;
	field[x][y] = len;

	/* output upper border */
	p = retval;
	*p++ = '+';
	for (i = 0; i < fld_x / 2; i++)
		*p++ = '-';
	for (i = p - retval - 1; i < fld_x; i++)
		*p++ = '-';
	*p++ = '+';
	*p++ = '\n';

	/* output content */
	for (y = 0; y < (ssize_t)fld_y; y++) {
		*p++ = '|';
		for (x = 0; x < (ssize_t)fld_x; x++)
			*p++ = augmentation_string[MIN(field[x][y], len)];
		*p++ = '|';
		*p++ = '\n';
	}

	/* output lower border */
	*p++ = '+';
	for (i = 0; i < fld_x; i++)
		*p++ = '-';
	*p++ = '+';

	/* show input line and return art */
	puts(userstr);
	return retval;
}

int 
main(int argc, char **argv)
{
	char	*line = NULL;
	size_t	line_buf_len = 0;
	ssize_t	line_len;
	int	delim = 10; // ASCII for newline
	ssize_t	usr_fldbase = 8;
	int	c;

	while ((c = getopt(argc, argv, "d:y:")) != -1)
		switch (c) {
		case 'd':
			if (strlen(optarg) > 1) {
				fprintf(stderr,
				"WARNING: delimiter must be one character long.\n");
			}
			delim = (int)*optarg;
			break;
		case 'y':
			if ((usr_fldbase = strtol_wrapper(optarg, NULL, 0)) <= 0) {
				fprintf(stderr,
				"ERROR: field base must be a hex, octal, or decimal number > 0 and < 128.\n");
				return 1;
			} else if (usr_fldbase >= 128) {
				fprintf(stderr,
				"ERROR: field base must be a hex, octal, or decimal number > 0 and < 128.\n");
				return 1;
			}
			break;
		default:
			break;
		}

	while ((line_len = getdelim(&line, &line_buf_len, delim, stdin)) > 0) {
		char	*randomart = NULL;
		if (line == NULL) {
			fprintf ( stderr,"ERROR: null pointer dereference of line\n" );
			return 1;
		} else if (line_len < 0) {
			fprintf(stderr, "ERROR: getdelim() returned -1\n");
		} else {
			line[line_len - 1] = '\0';
			randomart = fingerprint_randomart(line, (size_t)line_len - 1, (size_t)usr_fldbase);
		}

		if (randomart == NULL) {
			fprintf (stderr,"ERROR: fingerprint_randomart() returned NULL for input:\n%s\n", line);
			return 1;
		} else {
			memset(line, 0, (size_t)line_len);
			printf("%s\n",randomart);
		}
		free(randomart);
	}

	free(line);
	return error;
}
