/*
 * Adapted by Leo Famulari from work by Inge Henriksen, (c) 2008, 2009.
 * See LICENSE file for more information.
 *
 * 1. The following code is part of TM++, the "Software", is an embedded 
 * topic maps engine written in Standard C++.
 * 2. Copyright (C) 2007-2009 Inge Eivind Henriksen.
 *
 * 3. You are not allowed to use this software to make a database management 
 * system without the written consent of Inge Eivind Henriksen.
 *
 * 4. If not violating any of the conditions of paragraph 3, then this 
 * software is free software: you can redistribute it and/or modify it under 
 * the terms of the MIT License as published by the Open Source Initiative.
 *
 * 5. This software is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the MIT License for more details.
 *
 * 6. You should have received a copy of the The MIT License along with this 
 * software. If not, see <http://www.opensource.org/licenses>.
 */
unsigned char *
base64_d(const char *in)
{
	/* base64 decoding array  */
	const signed char alphabet[256] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
	52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,
	28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; 

	static unsigned char out[4] = {'\0'};
	out[3] = '\0';

	char buff[4];
	int i = 0, j;
	while ((buff[i] = in[i]) && (buff[i] != '=') && (buff[i] != '\0')) {
		if ( ++i == 4) {
			for (i = 0; i != 4; i++) 
				buff[i] = alphabet[(int)buff[i]];
			
			out[0] = ((buff[0] << 2) ^ ((buff[1] & 0x30) >> 4));  
			out[1] = (((buff[1] & 0x0f) << 4) ^ ((buff[2] & 0x3c) >> 2));
			out[2] = (((buff[2] & 0x03) << 6) ^ (buff[3] & 0x3f));

			return out;
		}
	}

	if (i) {
	
		for (j = 0; j < i; j++)
			buff[j] = alphabet[(int)buff[j]];
		for (j = i; j < 4; j++)
			buff[j] = '\0';

		out[0] = ((buff[0] << 2) ^ ((buff[1] & 0x30) >> 4));  
		out[1] = (((buff[1] & 0x0f) << 4) ^ ((buff[2] & 0x3c) >> 2));
		out[2] = (((buff[2] & 0x03) << 6) ^ (buff[3] & 0x3f));

		return out;
	}
	return out;
}
