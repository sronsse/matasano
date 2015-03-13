#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <base64.h>
#include <xor.h>

static uint8_t *cbc_encrypt(uint8_t *data, int len, uint8_t *key, int blk_size, int bit_size, uint8_t *iv)
{
	uint8_t *result;
	uint8_t *xor_blk;
	uint8_t *blk;
	AES_KEY aes_key;
	int i;

	/* Allocate result array */
	result = malloc(len * sizeof(uint8_t));

	/* Set key */
	AES_set_encrypt_key(key, bit_size, &aes_key);

	/* AES-CBC encrypt (using ECB cipher core) */
	for (i = 0; i < len / blk_size; i++) {
		/* Set block to XOR with (IV or previous block) */
		blk = (i == 0) ? iv : &result[(i - 1) * blk_size];

		/* XOR-encrypt data */
		xor_blk = xor_crypt(&data[i * blk_size],
			blk_size,
			blk,
			blk_size);

		/* Encrypt data */
		AES_encrypt(xor_blk,
			&result[i * blk_size],
			&aes_key);

		/* Free XOR block */
		free(xor_blk);
	}

	/* Return encrypted result */
	return result;
}

static uint8_t *cbc_decrypt(uint8_t *data, int len, uint8_t *key, int blk_size, int bit_size, uint8_t *iv)
{
	uint8_t *result;
	uint8_t *xor_blk;
	uint8_t *blk;
	AES_KEY aes_key;
	int i;

	/* Allocate result arrays */
	result = malloc(len * sizeof(uint8_t));
	xor_blk = malloc(blk_size * sizeof(uint8_t));

	/* Set key */
	AES_set_decrypt_key(key, bit_size, &aes_key);

	/* AES-ECB decrypt (using ECB cipher core) */
	for (i = 0; i < len / blk_size; i++) {
		/* Decrypt data */
		AES_decrypt(&data[i * blk_size],
			xor_blk,
			&aes_key);

		/* Set block to XOR with (IV or previous block) */
		blk = (i == 0) ? iv : &data[(i - 1) * blk_size];

		/* XOR-decrypt data */
		blk = xor_crypt(xor_blk,
			blk_size,
			blk,
			blk_size);

		/* Write back decrypted block */
		memcpy(&result[i * blk_size], blk, blk_size);

		/* Free blocks */
		free(blk);
	}

	/* Free array */
	free(xor_blk);

	/* Return decrypted result */
	return result;
}

void challenge10(char *filename, uint8_t *key, int blk_size, int bit_size)
{
	FILE *f;
	size_t size;
	char *str;
	char *line;
	size_t n;
	ssize_t read;
	int len;
	uint8_t *result;
	uint8_t *dec_a;
	uint8_t *dec_b;
	uint8_t *enc;
	uint8_t iv[blk_size];

	printf("*** Challenge 10: Implement CBC mode\n");

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

	/* Decode Base4 data */
	result = base64_decode(str, &len);

	/* Set initialization vector */
	memset(iv, 0, blk_size);

	/* Decrypt/encrypt/decrypt data using custom implementation */
	dec_a = cbc_decrypt(result, len, key, blk_size, bit_size, iv);
	enc = cbc_encrypt(dec_a, len, key, blk_size, bit_size, iv);
	dec_b = cbc_decrypt(enc, len, key, blk_size, bit_size, iv);

	/* Print ASCII representation */
	printf("ASCII: %.*s\n", len, dec_b);

	/* Free arrays */
	free(result);
	free(dec_a);
	free(dec_b);
	free(enc);
}

