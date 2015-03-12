#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void challenge9(char *str, int blk_size)
{
	int len;
	int padding_len;
	uint8_t *result;
	int i;

	printf("*** Challenge 9: Implement PKCS#7 padding\n");

	/* Print input and block size */
	printf("Input: %s\n", str);
	printf("Block size: %d\n", blk_size);

	/* Get data length */
	len = strlen(str);

	/* Compute padding length */
	padding_len = blk_size % len;

	/* PKCS#7 appends a new block when data is a multiple of block size */
	if (padding_len == 0)
		padding_len += blk_size;

	/* Allocate result */
	result = malloc((len + padding_len) * sizeof(uint8_t));

	/* Copy initial data */
	memcpy(result, str, len);

	/* Pad remaining bytes */
	for (i = 0; i < padding_len; i++)
		result[len + i] = padding_len;

	/* Print result */
	printf("Result: %s", str);
	for (i = 0; i < padding_len; i++)
		printf("\\x%02d", result[len + i]);
	printf("\n");

	/* Free result */
	free(result);
}

