#ifndef _SCORE_H
#define _SCORE_H

#include <stdint.h>

#define ENGLISH_COMMON "ETAOIN SHRDLU"

int get_score(uint8_t *data, int length, char *common);

#endif

