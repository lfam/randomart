#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>

static char *
fingerprint_randomart(u_char *dgst_raw, size_t dgst_raw_len,
		const struct sshkey *k)
