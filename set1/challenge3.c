#include <stdio.h>
#include <stdlib.h>
#include <hex.h>
#include <score.h>
#include <xor.h>

static uint8_t *decipher(uint8_t *data, int len, uint8_t *key)
{
	uint8_t *result;
	uint8_t *best_result;
	uint8_t k;
	int best_score;
	int score;

	/* Initialize score/result */
	best_score = -1;
	best_result = NULL;

	/* Decipher by cycling through 256 keys and saving the best one */
	k = 0;
	do {
		/* Decrypt data */
		result = xor_crypt(data, len, &k, 1);

		/* Save result if score is less than previous (better) */
		score = get_score(result, len, ENGLISH_COMMON);
		if ((best_score == -1) || (score < best_score)) {
			free(best_result);
			best_result = result;
			best_score = score;
			*key = k;
			continue;
		}

		/* Free result as its score was not better */
		free(result);

		/* Increment key */
		k++;
	} while (k != 0xFF);

	/* Return result with best score */
	return best_result;
}

void challenge3(char *hex)
{
	uint8_t *hex_bytes;
	uint8_t *result;
	int hex_len;
	uint8_t key = 0;

	printf("*** Challenge 3: Single-byte XOR cipher\n");

	/* Print input hex string */
	printf("Input: %s\n", hex);

	/* Convert input hex string to byte array */
	hex_bytes = hex_decode(hex, &hex_len);

	/* Decipher input */
	result = decipher(hex_bytes, hex_len, &key);

	/* Print out result with best score */
	printf("Result (obtained with 0x%02x key): %s\n", key, result);

	/* Free arrays */
	free(hex_bytes);
	free(result);
}

