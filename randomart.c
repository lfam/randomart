#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>

#include "strtol_wrap.h"
#include "base64_d.h"

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

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
 * Movement is taken from raw 2bit-wise.  Bumping into walls
 * makes the respective movement vector be ignored for this turn.
 * Graphs are not unambiguous, because circles in graphs can be
 * walked in either direction.
 */

char *fingerprint_randomart(unsigned char *raw, size_t raw_len, size_t fld_base, char *palette);
char * 
fingerprint_randomart(unsigned char *raw, size_t raw_len, size_t fldbase, char *palette)
{
	/*
	 * Chars to be used after each other every time the worm
	 * intersects with itself.  Matter of taste.
	 */
	char	*augmentation_string = " .o+=*BOX@%&#/^SE";
	if (palette != NULL) augmentation_string = palette;
	size_t	len = strlen(augmentation_string) - 1;

	char	*retval, *p;
	size_t	b;
	ssize_t	x, y;
	size_t  fld_x, fld_y;
	size_t	i;

	/*
	* Field sizes for the random art.  Have to be odd, so the starting point
	* can be in the exact middle of the picture.
	*/
	fld_x = fldbase * 2 + 1;
	fld_y = fldbase + 1;
	/* make odd */
	if ((fldbase & 1) == 1) {
		fld_x -= 2;
		fld_y--;
	}
	unsigned char field[fld_x][fld_y];
	
	if ((retval = calloc(((size_t)fld_x + 3), ((size_t)fld_y + 2))) == NULL) {
		perror("ERROR calloc()");
		return NULL;
	}

	/* initialize field */
	memset(field, 0, fld_x * fld_y * sizeof(char));
	x = fld_x / 2;
	y = fld_y / 2;

	/* process user's input */
	for (i = 0; i < raw_len; i++) {
		int input;
		input = raw[i];
		/* each input byte conveys four 2-bit move commands */
		for (b = 0; b < 4; b++) {
			/* evaluate 2 bit, rest is shifted later */
			x += (input & 0x1) ? 1 : -1;
			y += (input & 0x2) ? 1 : -1;

			/* assure we are still in bounds */
			x = MAX(x, 0);
			y = MAX(y, 0);
			x = MIN(x, (ssize_t)fld_x - 1);
			y = MIN(y, (ssize_t)fld_y - 1);

			/* augment the field */
			if (field[x][y] < len - 2)
				field[x][y]++;
			input = input >> 2;
		}
	}

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

	/* return art */
	return retval;
}

int 
main(int argc, char **argv)
{
	// Command-line parameter defaults
	int	delim = 10; // ASCII for newline
	char	*palette = NULL; // " .,\`;-~*x=#%&@WSE"
	long	radix = 16;
	long	fldbase = 8;

	int	c;
	while ((c = getopt(argc, argv, "0d:p:r:y:")) != -1)
	{
		switch (c) {
		case '0':
			delim = 0;
			break;
		case 'd':
			if (strlen(optarg) > 1) {
				fprintf(stderr,
				"WARNING: only first character of delimiter will be used.\n");
			}
			delim = (int)*optarg;
			break;
		case 'p':
			if (strlen(optarg) != 17) {
				fprintf(stderr,
				"WARNING: palette should be 17 characters long.\n");
			}
			palette = calloc(18, sizeof(char));
			memcpy(palette, optarg, 17);
			break;
		case 'r':
			strtol_wrap(&radix, optarg, 10, NULL);
			if ((radix != 16) && (radix != 64)) {
				fprintf(stderr,
				"ERROR: Radix must be 16 or 64.\n");
				return 1;

			}
			break;
		case 'y':
			strtol_wrap(&fldbase, optarg, 0, NULL);
			if ((fldbase < 1) || (fldbase > 127)) {
				fprintf(stderr,
				"ERROR: field base must be a hex, octal, or decimal integer > 0 and < 128.\n");
				return 1;
			}
			break;
		default:
			return 1;
		}
	}

	char	*line = NULL;
	size_t	line_buf_len = 0;
	ssize_t	line_len;
	while ((line_len = getdelim(&line, &line_buf_len, delim, stdin)) > 0) {
		if (line_len < 0) {
			perror("ERROR getdelim()");
			return 1;
		} else if (line == NULL) {
			fprintf(stderr,"ERROR: char *line is null after getdelim()\n");
			return 1;
		}
		assert(line_len > 0);

		/* remove delimiter from end of line */
		line[line_len - 1] = '\0';
		line_len--;

		/* set up error reporting for strtol() on user's input */
		char	*errstring = NULL;
		if ((errstring = malloc(line_len + 1)) == NULL) {
			perror("ERROR malloc()");
			return 1;
		}
		memset(errstring, ' ', line_len);
		errstring[line_len] = '\0';
		char 	*errptr = errstring;

		/* set up radix specific parameters */
		size_t nnums = 2;
		size_t raw_len = line_len / 2;
		switch (radix) {
			case 16:
				nnums = 2;
				raw_len = (line_len / 2);
				break;
			case 64:
				nnums = 4;
				raw_len = (((line_len / 4) * 3) + (line_len % 4 ? 3 : 0));
				break;
			default:
				break;
		}

		/* set up unsigned char array for processing */
		unsigned char *raw = NULL;
		if ((raw = calloc(raw_len + 1, sizeof(unsigned char))) == NULL) {
			perror("ERROR malloc()");
			return 1;
		}
		raw[raw_len] = '\0';
		unsigned char *rawp = raw;

		int i;
		for (i = 0; i < line_len; i += nnums) {
			/* break off enough characters to represent one 
			 * unit of input for the specified radix
			 */
			char num[nnums + 1];
			memset(num, '\0', sizeof(num));
			memcpy(num, &line[i], sizeof(num) - 1);

			/* process one unit of input */
			switch (radix) {
			case 16:
				;
				long strtol_ret;
				strtol_wrap(&strtol_ret, num, 16, &errptr);
				assert(strtol_ret < 256);
				errptr += strlen(num);
				*rawp = strtol_ret;
				rawp += sizeof(unsigned char);
				break;
			case 64:
				;
				unsigned char *decoded = base64_d(num);
				memcpy(rawp, decoded, 3);
				rawp += 3 * sizeof(unsigned char);
				break;
			default:
				break;
			}
		}

		errptr = errstring;
		while (*errptr != '\0') {
			if (!isspace(*errptr)) {
				fputs(errstring, stderr);
				fprintf(stderr, "<-- WARNING: not hexadecimal\n");
				break;
			}
			errptr++;
		}
		free(errstring);

		puts(line); // is this really necessary?

		char *randomart = NULL;
		if ((randomart = fingerprint_randomart(raw, raw_len, (size_t)fldbase, palette)) == NULL) {
			fprintf (stderr,"ERROR: fingerprint_randomart() returned NULL for input:\n");
			fputs(line, stderr);
			return 1;
		}
		memset(line, 0, (size_t)line_len);
		puts(randomart);

		free(randomart);
		free(raw);
	}

	free(palette);
	free(line);
	return 0;
}
