#ifndef _XOR_H
#define _XOR_H

#include <stdint.h>

uint8_t *xor_crypt(uint8_t *data, int data_len, uint8_t *key, int key_len);

#endif

