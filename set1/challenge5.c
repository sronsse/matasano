#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xor.h>

void challenge5(char *str, char *key)
{
	uint8_t *result;
	int str_len;
	int key_len;
	int i;

	printf("*** Challenge 5: Implement repeating-key XOR\n");

	/* Print inputs */
	printf("Input: %s\n", str);
	printf("Key: %s\n", key);

	/* Get input lengths */
	str_len = strlen(str);
	key_len = strlen(key);

	/* Encrypt input */
	result = xor_crypt((uint8_t *)str, str_len, (uint8_t *)key, key_len);

	/* Print out result */
	printf("Result: ");
	for (i = 0; i < str_len; i++)
		printf("%02x", result[i]);
	printf("\n");

	/* Free array */
	free(result);
}

