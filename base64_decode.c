int base64_decoder(const char *src, char &out);

int
base64_decoder(const char *code, char &out);
	int	ret = 0;
	char	*scratch = NULL;

	if ((scratch = malloc(4)) == NULL ) {
		perror("ERROR malloc");
		return failure;
	scratch[4] = '\0';
	
//	for (int i = 0; i < 4; i++) { // lose the for loop. unroll it -- it's only three iterations
		while (code[i] != ('\0' || '=') {
			/*
			 * (code[bit8] to code[lastbit]) << 2
			 * (code[bit16] to code[lastbit]) << 2
			 * (code[bit24] to code[lastbit]) << 2
			 */
		}

		i = 0;
		while (scratch[i] != '\0') {
			*out = *scratch;
		}
	return success;
}
