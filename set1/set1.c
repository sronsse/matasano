#include <stdio.h>

extern void challenge1(char *str);
extern void challenge2(char *hex, char *xor);
extern void challenge3(char *hex);
extern void challenge4(char *filename);
extern void challenge5(char *str, char *key);
extern void challenge6(char *filename);
extern void challenge7(char *f, unsigned char *key, int blk_size, int bit_size);
extern void challenge8(char *filename, int blk_size);

void set1()
{
	printf("****** Set 1\n");

	/* Run all challenges */
	challenge1("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
	challenge2("1c0111001f010100061a024b53535009181c", "686974207468652062756c6c277320657965");
	challenge3("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
	challenge4("files/4.txt");
	challenge5("Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal", "ICE");
	challenge6("files/6.txt");
	challenge7("files/7.txt", (unsigned char *)"YELLOW SUBMARINE", 16, 128);
	challenge8("files/8.txt", 16);
}

