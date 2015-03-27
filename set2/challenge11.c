#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/aes.h>

#define NUM_LOOPS	100
#define MODE_ECB	0
#define MODE_CBC	1
#define BLOCK_SIZE	16
#define BIT_SIZE	128

static uint8_t *gen_random_bytes(int len)
{
	uint8_t *bytes;
	int i;

	/* Allocate array */
	bytes = malloc(len * sizeof(uint8_t));

	/* Fill array random numbers */
	for (i = 0; i < len; i++)
		bytes[i] = rand() % 256;

	/* Return generated array */
	return bytes;
}

static uint8_t *encryption_oracle(uint8_t *in, int len, int *real, int *out_len)
{
	uint8_t *bytes;
	uint8_t *key;
	uint8_t *iv = NULL;
	uint8_t *rand1;
	uint8_t *rand2;
	uint8_t *enc;
	AES_KEY aes_key;
	int n1;
	int n2;
	int i;

	/* Generate random key */
	key = gen_random_bytes(BLOCK_SIZE);

	/* Choose two random numbers between 5 and 10 */
	n1 = rand() % 6 + 5;
	n2 = rand() % 6 + 5;

	/* Create new random arrays */
	rand1 = gen_random_bytes(n1);
	rand2 = gen_random_bytes(n2);

	/* Allocate new array */
	bytes = malloc((n1 + len + n2) * sizeof(uint8_t));

	/* Prepend/append random bytes to plain text */
	memcpy(bytes, rand1, n1 * sizeof(uint8_t));
	memcpy(&bytes[n1], in, len * sizeof(uint8_t));
	memcpy(&bytes[n1 + len], rand2, n2 * sizeof(uint8_t));

	/* Update array length */
	len += n1 + n2;

	/* Randomize encryption algorithm to use */
	*real = rand() % 2;

	/* Allocate encryption result (include possible padding) */
	*out_len = len + BLOCK_SIZE;
	enc = malloc(*out_len * sizeof(uint8_t));

	/* Set encryption key */
	AES_set_encrypt_key(key, BIT_SIZE, &aes_key);

	/* Encrypt data based on selected mechanism */
	if (*real == MODE_ECB) {
		for (i = 0; i < len / BLOCK_SIZE; i++)
			AES_encrypt(&bytes[i * BLOCK_SIZE],
				&enc[i * BLOCK_SIZE],
				&aes_key);
	} else {
		iv = gen_random_bytes(BLOCK_SIZE);
		AES_cbc_encrypt(bytes, enc, len, &aes_key, iv, AES_ENCRYPT);
	}

	/* Free arrays */
	free(bytes);
	free(key);
	free(iv);
	free(rand1);
	free(rand2);

	/* Returned encrypted result */
	return enc;
}

static int detect_mode(uint8_t *enc, int len)
{
	int i;
	int n = 0;

	/* Detect ECB if a block is fully repeated (assuming repeated input) */
	for (i = 0; i < len - BLOCK_SIZE; i++) {
		n = (enc[i] == enc[i + BLOCK_SIZE]) ? n + 1 : 0;
		if (n == BLOCK_SIZE)
			return MODE_ECB;
	}

	/* Assume CBC otherwise */
	return MODE_CBC;
}

void challenge11(char *str)
{
	uint8_t *enc;
	int out_len;
	int real;
	int detected;
	int num_ecb;
	int num_cbc;
	int successes;
	int len;
	int i;
	float percentage;

	printf("*** Challenge 11: An ECB/CBC detection oracle\n");

	/* Initialize random seed */
	srand(time(NULL));

	/* Get string length */
	len = strlen(str);

	/* Initialize test variables */
	successes = 0;
	num_ecb = 0;
	num_cbc = 0;

	/* Run test */
	for (i = 0; i < NUM_LOOPS; i++) {
		/* Encrypt data and detect encryption type */
		enc = encryption_oracle((uint8_t *)str, len, &real, &out_len);
		detected = detect_mode(enc, out_len);

		/* Increment appropriate test variable */
		if (real == MODE_ECB)
			num_ecb++;
		else
			num_cbc++;

		/* Increment number of successes if possible */
		if (real == detected)
			successes++;

		/* Free array */
		free(enc);
	}

	/* Print ECB percentage rate */
	percentage = 100 * (float)num_ecb / NUM_LOOPS;
	printf("ECB encryption rate: %.0f%%\n", percentage);

	/* Print CBC percentage rate */
	percentage = 100 * (float)num_cbc / NUM_LOOPS;
	printf("CBC encryption rate: %.0f%%\n", percentage);

	/* Print success percentage rate */
	percentage = 100 * (float)successes / NUM_LOOPS;
	printf("Detection success rate: %.0f%%\n", percentage);
}

