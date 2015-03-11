extern void challenge1(char *str);
extern void challenge2(char *hex, char *xor);

void set1()
{
	/* Run all challenges */
	challenge1("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
	challenge2("1c0111001f010100061a024b53535009181c", "686974207468652062756c6c277320657965");
}

