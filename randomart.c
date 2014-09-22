#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
 * Movement is taken from dgst_raw 2bit-wise.  Bumping into walls
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

int fingerprint_randomart(char *dgst_raw, size_t dgst_raw_len);

int
fingerprint_randomart(char *dgst_raw, size_t dgst_raw_len)
{
	/*
	 * Chars to be used after each other every time the worm
	 * intersects with itself.  Matter of taste.
	 */
	char	*augmentation_string = " .o+=*BOX@%&#/^SE";
	char	*retval, *p, title[FLDSIZE_X];
	unsigned char	field[FLDSIZE_X][FLDSIZE_Y];
	size_t	i, tlen;
	unsigned int	b;
	int	x, y;
/*	int	r; */
	size_t	len = strlen(augmentation_string) - 1;
	
	if ((retval = calloc((FLDSIZE_X + 3), (FLDSIZE_Y + 2))) == NULL)
		return 1;

	/* initialize field */
	memset(field, 0, FLDSIZE_X * FLDSIZE_Y * sizeof(char));
	x = FLDSIZE_X / 2;
	y = FLDSIZE_Y / 2;

	/* process raw key */

	for (i = 0; i < dgst_raw_len; i+=2) {
		long input = 0;
		char pair_of_chars[2];

		memset(pair_of_chars,0,2 * sizeof(int));
		memcpy(pair_of_chars,&dgst_raw[i],sizeof(pair_of_chars));

		input = strtol(pair_of_chars,NULL,16);

		/* each byte conveys four 2-bit move commands */
		for (b = 0; b < 4; b++) {


			/* evaluate 2 bit, rest is shifted later */
			x += (input & 0x1) ? 1 : -1;
			y += (input & 0x2) ? 1 : -1;

			/* assure we are still in bounds */
			x = MAX(x, 0);
			y = MAX(y, 0);
			x = MIN(x, FLDSIZE_X - 1);
			y = MIN(y, FLDSIZE_Y - 1);

			/* augment the field */
			if (field[x][y] < len - 2)
				field[x][y]++;
			input = input >> 2;
		}
	}

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
	memcpy(p, title, tlen);
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

	printf("%s\n",retval);
	free(retval);
	printf("input was %s\n",dgst_raw);
	return 0;
}

int 
main(void)
{
/* cribbed from http://www.pixelbeat.org/programming/readline/getline.c */
	char*	line = 0;
	size_t	line_buf_len=0;
	ssize_t	line_len;
	ssize_t	rart_input_len;

	while ((line_len = getline(&line, &line_buf_len, stdin)) > 0) {
		if ((line)[line_len - 1] == '\n') {
			rart_input_len = line_len - 1;
		} else {
			rart_input_len = line_len;
		}
		fingerprint_randomart(line,rart_input_len);
		memset(line,0,line_len);
	}

	free(line);

	return 0;
}
