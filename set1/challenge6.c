#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base64.h>
#include <score.h>
#include <xor.h>

#define KEYSIZE_MIN 2
#define KEYSIZE_MAX 40

static int hamming(uint8_t *a, uint8_t *b, int len)
{
	int result;
	uint8_t data_a;
	uint8_t data_b;
	int i;
	int j;

	/* Cycle through all bits and compute bit difference */
	result = 0;
	for (i = 0; i < len; i++) {
		data_a = a[i];
		data_b = b[i];
		for (j = 0; j < 8; j++) {
			result += (data_a & 0x01) ^ (data_b & 0x01);
			data_a >>= 1;
			data_b >>= 1;
		}
	}

	/* Return hamming distance */
	return result;
}

static int find_key_size(uint8_t *data, int len)
{
	int best_key_size;
	int key_size;
	int pos;
	float best_score;
	float score;
	int hamming_res;
	int iterations;

	/* Initialize position and best score (to worst) */
	pos = 0;
	best_score = len;
	best_key_size = KEYSIZE_MIN;

	/* Cycle through defined possible key sizes */
	for (key_size = KEYSIZE_MIN; key_size <= KEYSIZE_MAX; key_size++) {
		/* Run until and of data is reached */
		score = 0.0f;
		iterations = 0;
		for (pos = 0; pos <= len - 2 * key_size; pos += key_size) {
			/* Compute hamming distance between two chunks */
			hamming_res = hamming(&data[pos],
				&data[pos + key_size],
				key_size);

			/* Normalize result and update score */
			score += (float)hamming_res / key_size;

			/* Increase iteration counter */
			iterations++;
		}

		/* Stop processing if no iterations were made */
		if (iterations == 0)
			break;

		/* Normalize score based on number of iterations */
		score /= (float)iterations;

		/* Update best score if needed */
		if (score < best_score) {
			best_key_size = key_size;
			best_score = score;
		}
	}

	/* Return best key size */
	return best_key_size;
}

static uint8_t find_key(uint8_t *data, int len)
{
	uint8_t *result;
	int best_score;
	int score;
	uint8_t key = 0;
	uint8_t k = 0;

	/* Decrypt string by cycling through 256 keys */
	best_score = -1;
	while (k != 0xFF) {
		/* Decrypt data */
		result = xor_crypt(data, len, &k, 1);

		/* Save result if score is better than previous (smaller) */
		score = get_score(result, len, ENGLISH_COMMON);
		if ((best_score == -1) || (score < best_score)) {
			best_score = score;
			key = k;
		}

		/* Free result */
		free(result);

		/* Increment key */
		k++;
	}

	/* Return best key */
	return key;
}

static uint8_t *decipher(uint8_t *data, int len, int *key_size)
{
	uint8_t *key;
	uint8_t *chunk;
	int chunk_size;
	int c;
	int i;

	/* Find key size from resulting data */
	*key_size = find_key_size(data, len);

	/* Allocate key */
	key = malloc(*key_size * sizeof(uint8_t));

	/* Set chunk size */
	chunk_size = len / *key_size;

	/* Allocate temporary chunk */
	chunk = malloc(chunk_size * sizeof(uint8_t));

	/* Try deciphering all characters of entire key */
	for (c = 0; c < *key_size; c++) {
		/* Build chunk */
		for (i = 0; i < chunk_size; i++)
			chunk[i] = data[i * *key_size + c];

		/* Single-XOR decipher */
		key[c] = find_key(chunk, chunk_size);
	}

	/* Return best key */
	return key;
}

void challenge6(char *filename)
{
	FILE *f;
	size_t size;
	char *str;
	char *line;
	uint8_t *key;
	uint8_t *result;
	uint8_t *dec;
	size_t n;
	ssize_t read;
	int key_size;
	int len;

	printf("*** Challenge 6: Break repeating-key XOR\n");

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

	/* Find key */
	key = decipher(result, len, &key_size);
	printf("Best key size: %u\n", key_size);
	printf("Key: %.*s\n", key_size, key);

	/* Decrypt data */
	dec = xor_crypt(result, len, key, key_size);

	/* Print ASCII representation */
	printf("ASCII: %s\n", dec);

	/* Free arrays */
	free(str);
	free(result);
	free(dec);
}

