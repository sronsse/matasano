#include <stdint.h>
#include <stdio.h>

extern void challenge9(char *str, int blk_size);
extern void challenge10(char *f, uint8_t *key, int blk_size, int bit_size);

void set2()
{
	printf("****** Set 2\n");

	/* Run all challenges */
	challenge9("YELLOW SUBMARINE", 20);
	challenge10("files/10.txt", (uint8_t *)"YELLOW SUBMARINE", 16, 128);
}

