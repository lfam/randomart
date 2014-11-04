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

/* function prototypes */
char *fingerprint_randomart(char *userstr, size_t userstr_len, size_t usr_fldbase);

char * 
fingerprint_randomart(char *userstr, size_t userstr_len, size_t usr_fldbase) {
	/*
	 * Chars to be used after each other every time the worm
	 * intersects with itself.  Matter of taste.
	 */
	char	*augmentation_string = " .o+=*BOX@%&#/^SE";
	char	*retval, *p;
//	char	title[fld_y];
	size_t	tlen;
	size_t	b;
	size_t	x, y;
//	int	r;
	size_t	len = strlen(augmentation_string) - 1;
	size_t fld_x, fld_y;
	size_t	i;

	/*
	* Field sizes for the random art.  Have to be odd, so the starting point
	* can be in the exact middle of the picture, and FLDBASE should be >=8 .
	* Else pictures would be too dense, and drawing the frame would
	* fail, too, because the key type would not fit in anymore.
	*/
	fld_y = usr_fldbase + 1;
	fld_x = usr_fldbase * 2 + 1;

	unsigned char	field[fld_x][fld_y];
	
	if ((retval = calloc((fld_x + 3), (fld_y + 2))) == NULL)
		return NULL;

	/* initialize field */
	memset(field, 0, fld_x * fld_y * sizeof(char));
	x = fld_x / 2;
	y = fld_y / 2;

	char	*errstring;
	if ((errstring = malloc(userstr_len)) == NULL)
		return NULL;
	memset(errstring, ' ', userstr_len);
	errstring[userstr_len - 1] = '\0';

	char 	*errptr = errstring;
	int	strtoul_err = 0;

	/* process raw key */
	for (i = 0; i < userstr_len; i+=2) {

		/* break off two characters (hex number) */
		char	num_as_str[3];
		memset(num_as_str, 0, sizeof(num_as_str));
		memcpy(num_as_str, &userstr[i], sizeof(num_as_str) - 1);
		
		/*
		 * (unsigned long)input should be =< 255, i.e. it must fit in one byte.
		 * This works here because the max value of 2 chars read as
		 * base16 is 255, which is the max value of 8 bits.
		 * This will need to change when we allow other radices.
		 */
		unsigned char	byte = '\0';
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
			continue;
		} else if ('\0' != *end) {
				*errptr = *num_as_str;
				size_t idx = 0;
				while (idx < strlen(num_as_str)) {
				idx++;
				*errptr++;
				}
			strtoul_err = 1 ;
			continue;
		} else if (input > UINT_MAX) {
			fprintf(stderr, "%lu greater than UINT_MAX\n", input);
			fprintf(stderr, "Not all input will be processed.\n");
			strtoul_err = 1 ;
			continue;
		} else {
			errptr += strlen(num_as_str);
			byte = (unsigned char)input;
		}

		/* each byte conveys four 2-bit move commands */
		for (b = 0; b < 4; b++) {


			/* evaluate 2 bit, rest is shifted later */
			x += (byte & 0x1) ? 1 : -1;
			y += (byte & 0x2) ? 1 : -1;

			/* assure we are still in bounds */
			x = MAX(x, 0);
			y = MAX(y, 0);
			x = MIN(x, fld_x - 1);
			y = MIN(y, fld_y - 1);

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
	field[fld_x / 2][fld_y / 2] = len - 1;
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
	for (i = 0; i < (fld_x - tlen) / 2; i++)
		*p++ = '-';
//	memcpy(p, title, tlen);
	p += tlen;
	for (i = p - retval - 1; i < fld_x; i++)
		*p++ = '-';
	*p++ = '+';
	*p++ = '\n';

	/* output content */
	for (y = 0; y < fld_y; y++) {
		*p++ = '|';
		for (x = 0; x < fld_x; x++)
			*p++ = augmentation_string[MIN(field[x][y], len)];
		*p++ = '|';
		*p++ = '\n';
	}

	/* output lower border */
	*p++ = '+';
	for (i = 0; i < fld_x; i++)
		*p++ = '-';
	*p++ = '+';

	fputs(userstr, stdout);
	return retval;
}

int 
main(int argc, char **argv)
{
	char	*line = NULL;
	size_t	line_buf_len = 0;
	ssize_t	line_len;
	char	*randomart = NULL;
	size_t	usr_fldbase;

	if (argc > 1) {
		usr_fldbase = (size_t)strtoul(argv[1], NULL, 10);
	} else {
		usr_fldbase = 8;
	}

	/* cribbed from http://www.pixelbeat.org/programming/readline/getline.c */
	while ((line_len = getline(&line, &line_buf_len, stdin)) > 0) {
		if (line == NULL) {
			fprintf ( stderr,"null pointer dereference of line\n" );
			return 1;
		} else if ((line)[line_len - 1] == '\n') {
			randomart = fingerprint_randomart(line, (size_t)line_len - 1, usr_fldbase);
		} else {
			randomart = fingerprint_randomart(line, (size_t)line_len, usr_fldbase);
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
