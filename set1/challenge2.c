#include <stdio.h>
#include <stdlib.h>
#include <hex.h>
#include <xor.h>

void challenge2(char *hex, char *xor)
{
	uint8_t *hex_bytes;
	uint8_t *xor_bytes;
	uint8_t *result;
	int hex_len;
	int xor_len;
	int i;

	printf("*** Challenge 2: Fixed XOR\n");

	/* Print input hex string */
	printf("Input hex: %s\n", hex);
	printf("Input xor: %s\n", xor);

	/* Convert input hex string to byte array */
	hex_bytes = hex_decode(hex, &hex_len);

	/* Convert input xor string to byte array */
	xor_bytes = hex_decode(xor, &xor_len);

	/* Print ASCII xor string representation */
	printf("ASCII (xor input): %s\n", xor_bytes);

	/* XOR encrypt data */
	result = xor_crypt(hex_bytes, hex_len, xor_bytes, xor_len);

	/* Print out result */
	printf("Result: ");
	for (i = 0; i < hex_len; i++)
		printf("%02x", result[i]);
	printf("\n");

	/* Free arrays */
	free(hex_bytes);
	free(xor_bytes);
	free(result);
}

