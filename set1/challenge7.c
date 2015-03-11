#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <base64.h>

void challenge7(char *filename, unsigned char *key, int blk_size, int bit_size)
{
	FILE *f;
	long size;
	char *line;
	char *str;
	uint8_t *result;
	uint8_t *dec;
	AES_KEY dec_key;
	ssize_t read;
	size_t n;
	int len;
	int i;

	printf("*** Challenge 7: AES in ECB mode\n");

	/* Open file */
	f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "Could not open \"%s\"!\n", filename);
		return;
	}

	/* Get file size */
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	/* Allocate string */
	str = malloc(size);

	/* Read input file removing new lines */
	len = 0;
	line = NULL;
	n = 0;
	while ((read = getline(&line, &n, f)) > 0) {
		strncpy(&str[len], line, read - 1);
		len += read - 1;
		free(line);
		line = NULL;
		n = 0;
	}

	/* Close file */
	fclose(f);

	/* Convert Base64 to byte array */
	result = base64_decode(str, &len);

	/* Allocate decryption array */
	dec = malloc(len * sizeof(uint8_t));

	/* Decrypt data using OpenSSL */
	AES_set_decrypt_key(key, bit_size, &dec_key);
	for (i = 0; i < len / blk_size; i++)
		AES_decrypt(&result[i * blk_size],
			&dec[i * blk_size],
			&dec_key);

	/* Print ASCII representation */
	printf("ASCII: %.*s\n", len, dec);

	/* Free arrays */
	free(str);
	free(result);
	free(dec);
}

