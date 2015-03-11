#ifndef _BASE_64_H
#define _BASE_64_H

#include <stdint.h>

char *base64_encode(uint8_t *data, int len);
uint8_t *base64_decode(char *data, int *out_len);

#endif

