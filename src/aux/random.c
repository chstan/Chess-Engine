#include <time.h>
#include <stdlib.h>

#include "random.h"
#include "../extglobals.h"

#define ACTUALLY_RANDOM

void initRandom() {
	#ifdef ACTUALLY_RANDOM
	srand(time(NULL));
	#else
	srand(0);
	#endif
	initRandom = true;
}

int random() {
	random(0, 2);
}

int random(const int limit) {
	random(0, limit);
}

int random(const int lowerLimit, const int upperLimit) {
	assert(initRandom);
	return lowerLimit + (rand() % upperLimit);
}

float randomf() {
	assert(initRandom);
	unsigned int bits = rand();
	float f = *(float *)&bits;
	return f;
}