#include <stdio.h>
#include <stdlib.h>
#include <base64.h>
#include <hex.h>

void challenge1(char *str)
{
	uint8_t *bytes;
	int len;

	printf("*** Challenge 1: Convert hex to base64\n");

	/* Print input hex string */
	printf("Input: %s\n", str);

	/* Convert input string to byte array */
	bytes = hex_decode(str, &len);

	/* Print ASCII representation */
	printf("ASCII: %s\n", bytes);

	/* Convert byte array to Base64 and print it */
	str = base64_encode(bytes, len);
	printf("Base64: %s\n", str);

	/* Free arrays */
	free(bytes);
}

