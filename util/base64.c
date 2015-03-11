#include <stdlib.h>
#include <string.h>
#include <base64.h>

static char index_to_b64(uint8_t b)
{
	char res;

	/* Map 6-bit value to Base64 encoding */
	if (b <= 25)
		res = 'A' + b;
	else if ((b >= 26) && (b <= 51))
		res = 'a' + (b - 26);
	else if ((b >= 52) && (b <= 61))
		res = '0' + (b - 52);
	else if (b == 62)
		res = '+';
	else
		res = '/';
	return res;
}

static uint8_t b64_to_index(char c)
{
	uint8_t res = 0;

	/* Map Base64 character to value */
	if ((c >= 'A') && (c <= 'Z'))
		res = c - 'A';
	else if ((c >= 'a') && (c <= 'z'))
		res = c - 'a' + 26;
	else if ((c >= '0') && (c <= '9'))
		res = c - '0' + 52;
	else if (c == '+')
		res = 62;
	else if (c == '/')
		res = 63;

	return res;
}

char *base64_encode(uint8_t *data, int len)
{
	char *str;
	char b[4];
	int output_len;
	int c = 0;
	int i;
	int j;

	/* Validate parameters */
	if (data == NULL)
		return NULL;

	/* Compute output string length */
	output_len = (len / 3) * 4 + 1;
	if (len % 3 != 0)
		output_len += 4;

	/* Allocate output string */
	str = malloc(output_len * sizeof(char));

	/* Parse byte array */
	for (i = 0; i < len / 3; i++) {
		b[0] = data[i * 3] >> 2;
		b[1] = ((data[i * 3] & 0x03) << 4) | (data[i * 3 + 1] >> 4);
		b[2] = ((data[i * 3 + 1] & 0x0F) << 2) | (data[i * 3 + 2] >> 6);
		b[3] = data[i * 3 + 2] & 0x3F;

		/* Map values */
		for (j = 0; j < 4; j++)
			str[c++] = index_to_b64(b[j]);
	}

	/* Handle remainder of 1 byte (producing 2 6-bit values) */
	if (len % 3 == 1) {
		b[0] = data[i * 3] >> 2;
		b[1] = (data[i * 3] & 0x03) << 4;

		/* Map values */
		for (j = 0; j < 2; j++)
			str[c++] = index_to_b64(b[j]);

		/* Add padding */
		str[c++] = '=';
		str[c++] = '=';
	}

	/* Handle remainder of 2 bytes (producing 3 6-bit values) */
	if (len % 3 == 2) {
		b[0] = data[i * 3] >> 2;
		b[1] = ((data[i * 3] & 0x03) << 4) | (data[i * 3 + 1] >> 4);
		b[2] = (data[i * 3 + 1] & 0x0F) << 2;

		/* Map values */
		for (j = 0; j < 3; j++)
			str[c++] = index_to_b64(b[j]);

		/* Add padding */
		str[c++] = '=';
	}

	/* Terminate and return converted string */
	str[c] = '\0';
	return str;
}

uint8_t *base64_decode(char *data, int *len)
{
	uint8_t *res;
	int data_len;
	uint8_t b[4];
	int count;
	int i;
	char c;

	/* Get data length and allocate result */
	data_len = strlen(data);
	res = malloc((data_len / 4) * 3 * sizeof(uint8_t));

	/* Convert Base64 encoding */
	count = 0;
	*len = 0;
	for (i = 0; i < data_len; i++) {
		/* Get character */
		c = data[i];

		/* Get value from Base64 */
		b[count++] = c;

		/* Produce output values if needed */
		if (count == 4) {
			/* Output first character */
			b[0] = b64_to_index(b[0]);
			b[1] = b64_to_index(b[1]);
			res[(*len)++] = (b[0] << 2) | (b[1] >> 4);

			/* Output second character if needed */
			if (b[2] != '=') {
				b[2] = b64_to_index(b[2]);
				res[(*len)++] = (b[1] << 4) | (b[2] >> 2);
			}

			/* Output third character if needed */
			if (b[3] != '=') {
				b[3] = b64_to_index(b[3]);
				res[(*len)++] = (b[2] << 6) | b[3];
			}

			/* Reset count */
			count = 0;
		}
	}

	/* Return converted result */
	return res;
}

