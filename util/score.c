#include <ctype.h>
#include <string.h>
#include <score.h>

int get_score(uint8_t *data, int length, char *common)
{
	int i;
	int j;
	int score = 0;
	int common_len;
	char c;

	/* Get amount of common characters */
	common_len = strlen(common);

	/* Assign score based common letters */
	for (i = 0; i < length; i++) {
		/* Convert input to upper case */
		c = toupper(data[i]);

		/* Check against common letters array */
		for (j = 0; j < common_len; j++)
			if (c == common[j]) {
				score += j;
				break;
			}

		/* Add max score if no character was found */
		if (j == common_len)
			score += common_len;
	}

	/* Return computed score */
	return score;
}

