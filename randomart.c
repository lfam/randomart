#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

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
 * Movement is taken from userstr 2bit-wise.  Bumping into walls
 * makes the respective movement vector be ignored for this turn.
 * Graphs are not unambiguous, because circles in graphs can be
 * walked in either direction.
 */

/*
 * Field sizes for the random art.  Have to be odd, so the starting point
 * can be in the exact middle of the picture, and FLDBASE should be >=8 .
 * Else pictures would be too dense, and drawing the frame would
 * fail, too, because the key type would not fit in anymore.
 */
#define	FLDBASE		8
#define	FLDSIZE_Y	(FLDBASE + 1)
#define	FLDSIZE_X	(FLDBASE * 2 + 1)

/* function prototypes */
char *fingerprint_randomart(char *userstr, size_t userstr_len);

char * 
fingerprint_randomart(char *userstr, size_t userstr_len) {
	/*
	 * Chars to be used after each other every time the worm
	 * intersects with itself.  Matter of taste.
	 */
	char	*augmentation_string = " .o+=*BOX@%&#/^SE";
	char	*retval, *p;
//	char	title[FLDSIZE_Y];
	unsigned char	field[FLDSIZE_X][FLDSIZE_Y];
	size_t	i, tlen;
	unsigned int	b;
	int	x, y;
//	int	r;
	size_t	len = strlen(augmentation_string) - 1;
	
	if ((retval = calloc((FLDSIZE_X + 3), (FLDSIZE_Y + 2))) == NULL)
		return NULL;

	/* initialize field */
	memset(field, 0, FLDSIZE_X * FLDSIZE_Y * sizeof(char));
	x = FLDSIZE_X / 2;
	y = FLDSIZE_Y / 2;

	char	*errstring;
	if ((errstring = malloc(userstr_len)) == NULL)
		return NULL;
	char 	*errptr = errstring;
	int	strtoul_err = 0;

	/* process raw key */
	for (i = 0; i < userstr_len; i+=2) {

		/* break off two characters (hex number) */
		char	num_as_str[3];
		memset(num_as_str, 0, sizeof(num_as_str));
		memcpy(num_as_str, &userstr[i], sizeof(num_as_str) - 1);
		
/*		int byte = strtoul_wrapper(num_as_str, errptr);
		if (byte == -1) continue;
*/
		/*
		 * (unsigned long)input should be =< 255, i.e. it must fit in one byte.
		 * This works here because the max value of 2 chars read as
		 * base16 is 255, which is the max value of 8 bits.
		 * This will need to change when we allow other radices.
		 */
		unsigned char	byte;
		unsigned long 	input;
		char	*end;
		input = strtoul(num_as_str,&end,16);

		/* adapted from
		* https://www.securecoding.cert.org/confluence/display/seccode/INT06-C.+Use+strtol%28%29+or+a+related+function+to+convert+a+string+token+to+an+integer
		*/
		if (end == num_as_str) {
			do {
				*errptr = *end;
				*errptr++;
				*end++;
			} while ( *end != '\0' ) ;
			strtoul_err = 1 ;
		} else if ('\0' != *end) {
			do {
				*errptr = *end;
				*errptr++;
				*end++;
			} while ( *end != '\0' ) ;
			strtoul_err = 1 ;
		} else if (input > UINT_MAX) {
			fprintf(stderr, "%lu greater than UINT_MAX\n", input);
			fprintf(stderr, "Not all input will be processed.\n");
			strtoul_err = 1 ;
		} else {
			memset(errptr,' ',strlen(num_as_str)); 
			errptr += strlen(num_as_str);
			byte = (unsigned char)input;
		}

		if ( strtoul_err == 1 ) {
			continue ; 
		}


		/* each byte conveys four 2-bit move commands */
		for (b = 0; b < 4; b++) {


			/* evaluate 2 bit, rest is shifted later */
			x += (byte & 0x1) ? 1 : -1;
			y += (byte & 0x2) ? 1 : -1;

			/* assure we are still in bounds */
			x = MAX(x, 0);
			y = MAX(y, 0);
			x = MIN(x, FLDSIZE_X - 1);
			y = MIN(y, FLDSIZE_Y - 1);

			/* augment the field */
			if (field[x][y] < len - 2)
				field[x][y]++;
			byte = byte >> 2;
		}
	}

	if ( strtoul_err != 0 ) {
		fputs(errstring, stderr);
		fprintf(stderr, "<-- input characters failed processing\n");
	}

	free(errstring);

	/* mark starting point and end point*/
	field[FLDSIZE_X / 2][FLDSIZE_Y / 2] = len - 1;
	field[x][y] = len;

	/* assemble title */
/*	r = snprintf(title, sizeof(title), "[%s %u]",
		sshkey_type(k), sshkey_size(k));
*/	/* If [type size] won't fit, then try [type]; fits "[ED25519-CERT]" */
/*	if (r < 0 || r > (int)sizeof(title))
		snprintf(title, sizeof(title), "[%s]", sshkey_type(k));
	tlen = strlen(title);
*/
	/* tlen is defined here to fix a compiler warning */
	tlen = 0;

	/* output upper border */
	p = retval;
	*p++ = '+';
	for (i = 0; i < (FLDSIZE_X - tlen) / 2; i++)
		*p++ = '-';
//	memcpy(p, title, tlen);
	p += tlen;
	for (i = p - retval - 1; i < FLDSIZE_X; i++)
		*p++ = '-';
	*p++ = '+';
	*p++ = '\n';

	/* output content */
	for (y = 0; y < FLDSIZE_Y; y++) {
		*p++ = '|';
		for (x = 0; x < FLDSIZE_X; x++)
			*p++ = augmentation_string[MIN(field[x][y], len)];
		*p++ = '|';
		*p++ = '\n';
	}

	/* output lower border */
	*p++ = '+';
	for (i = 0; i < FLDSIZE_X; i++)
		*p++ = '-';
	*p++ = '+';

	fputs(userstr, stdout);
	return retval;
}

int 
main(void)
{
	char	*line = 0;
	size_t	line_buf_len = 0;
	ssize_t	line_len;
	char	*randomart = NULL;

	/* cribbed from http://www.pixelbeat.org/programming/readline/getline.c */
	while ((line_len = getline(&line, &line_buf_len, stdin)) > 0) {
		if (line == NULL) {
			fprintf ( stderr,"null pointer dereference of line\n" );
			return 1;
		} else if ((line)[line_len - 1] == '\n') {
			randomart = fingerprint_randomart(line, (size_t)line_len - 1);
		} else {
			randomart = fingerprint_randomart(line, (size_t)line_len);
		}

		if (randomart == NULL) {
			fprintf (stderr,"fingerprint_randomart() returned NULL for input %s\n", line);
			return 1;
		} else {
			memset(line, 0, (size_t)line_len);
			printf("%s\n",randomart);
		}
	}

	free(randomart);
	free(line);

	return 0;
}
