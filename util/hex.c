#include <hex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t *hex_decode(char *str, int *len)
{
	uint8_t *data;
	unsigned int b;
	int i;

	/* Validate parameters */
	if ((str == NULL) || (len == NULL))
		return NULL;

	/* Compute output length */
	*len = strlen(str) / 2;

	/* Allocate output data */
	data = malloc(*len * sizeof(uint8_t));

	/* Build byte array */
	for (i = 0; i < *len; i++) {
		sscanf(&str[i * 2], "%02x", &b);
		data[i] = b;
	}

	/* Return byte array */
	return data;
}

