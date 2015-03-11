#include <stdlib.h>
#include <xor.h>

uint8_t *xor_encrypt(uint8_t *hex_bytes, uint8_t *xor_bytes, int length)
{
	uint8_t *result;
	int i;

	/* Validate parameters */
	if ((hex_bytes == NULL) || (xor_bytes == NULL))
		return NULL;

	/* Allocate result */
	result = malloc(length * sizeof(uint8_t));

	/* Encrypt input data */
	for (i = 0; i < length; i++)
		result[i] = hex_bytes[i] ^ xor_bytes[i];

	/* Return encrypted data */
	return result;
}

