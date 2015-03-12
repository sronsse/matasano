#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hex.h>

static int get_matches(uint8_t *data, int len, int blk_size)
{
	int num_matches;
	int i;
	int j;
	int k;
	uint8_t data_a;
	uint8_t data_b;

	/* Compare each block to each other and compute score */
	num_matches = 0;
	for (i = 0; i < len / blk_size; i++)
		for (j = 0; j < len / blk_size; j++)
			for (k = 0; k < blk_size; k++) {
				data_a = data[i * blk_size + k];
				data_b = data[j * blk_size + k];
				num_matches += (data_a == data_b);
			}

	/* Return number of matches */
	return num_matches;
}

void challenge8(char *filename, int blk_size)
{
	FILE *f;
	char *str;
	uint8_t *data;
	ssize_t read;
	size_t n;
	int score;
	int line;
	int best_score;
	int best_line;
	int len;

	printf("*** Challenge 8: Detect AES in ECB mode\n");

	/* Open file */
	f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "Could not open \"%s\"!\n", filename);
		return;
	}

	/* Initialize score/line */
	best_score = -1;
	best_line = -1;
	line = 1;

	/* Parse file and find string most likely encoded using AES-EBC */
	str = NULL;
	n = 0;
	while ((read = getline(&str, &n, f)) > 0) {
		/* Remove new line */
		str[read - 1] = '\0';

		/* Decode string */
		data = hex_decode(str, &len);

		/* Get number of byte matches in current line */
		score = get_matches(data, len, blk_size);

		/* Print score and save it if better than previous (higher) */
		printf("Line %d score: %d\n", line, score);
		if ((best_score == -1) || (score > best_score)) {
			best_score = score;
			best_line = line;
		}

		/* Increment line number */
		line++;

		/* Free arrays */
		free(str);
		free(data);
		str = NULL;
		n = 0;
	}

	/* Close file */
	fclose(f);

	/* Print ASCII representation */
	printf("AES-ECB encoded line: %d\n", best_line);
}

