#include <stdio.h>
#include <stdlib.h>
#include <hex.h>
#include <score.h>
#include <xor.h>

static uint8_t *decipher(uint8_t *data, int len, uint8_t *key, int *best_score)
{
	uint8_t *result;
	uint8_t *best_result;
	uint8_t k;
	int score;

	/* Initialize score/result */
	*best_score = -1;
	best_result = NULL;

	/* Decipher by cycling through 256 keys and saving the best one */
	k = 0;
	do {
		/* Decrypt data */
		result = xor_crypt(data, len, &k, 1);

		/* Save result if score is less than previous (better) */
		score = get_score(result, len, ENGLISH_COMMON);
		if ((*best_score == -1) || (score < *best_score)) {
			free(best_result);
			best_result = result;
			*best_score = score;
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

void challenge4(char *filename)
{
	FILE *f;
	char *str = NULL;
	size_t n = 0;
	uint8_t *bytes;
	uint8_t *best_result;
	uint8_t *result;
	int best_score;
	int score;
	int len;
	uint8_t key;
	uint8_t best_key;
	int best_len;

	printf("*** Challenge 4: Detect single-character XOR\n");

	/* Open file */
	f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "Could not open \"%s\"!\n", filename);
		return;
	}

	/* Initialize best score/result/key/len */
	best_score = -1;
	best_result = NULL;
	best_key = 0;
	best_len = 0;

	/* Parse all strings within file */
	while (getline(&str, &n, f) > 0) {
		/* Convert hex string to byte array */
		bytes = hex_decode(str, &len);

		/* Free string */
		free(str);
		str = NULL;
		n = 0;

		/* Decipher line getting best result */
		result = decipher(bytes, len, &key, &score);

		/* Free array */
		free(bytes);

		/* Save result if score is less than previous (better) */
		if ((best_score == -1) || (score < best_score)) {
			free(best_result);
			best_result = result;
			best_score = score;
			best_key = key;
			best_len = len;
			continue;
		}

		/* Free result as its score was not better */
		free(result);
	}

	/* Print out result with best score */
	printf("Result (0x%02x key): %.*s\n", best_key, best_len, best_result);

	/* Close file */
	fclose(f);

	/* Free arrays */
	free(best_result);
}

