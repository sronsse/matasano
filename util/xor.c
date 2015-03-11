#include <stdlib.h>
#include <xor.h>

uint8_t *xor_crypt(uint8_t *data, int data_len, uint8_t *key, int key_len)
{
	uint8_t *result;
	int pos;
	int i;

	/* Validate parameters */
	if (!data || !key)
		return NULL;

	/* Allocate result */
	result = malloc(data_len * sizeof(uint8_t));

	/* Encrypt input data */
	pos = 0;
	for (i = 0; i < data_len; i++) {
		result[i] = data[i] ^ key[pos];
		pos = (pos + 1) % key_len;
	}

	/* Return encrypted data */
	return result;
}

