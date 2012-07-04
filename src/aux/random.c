#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "random.h"
#include "../extglobals.h"

#define ACTUALLY_RANDOM

void initRandom() {
	#ifdef ACTUALLY_RANDOM
	srand(time(NULL));
	#else
	srand(0);
	#endif
	randomInitialized = true;
}

int randomi(const int lowerLimit, const int upperLimit) {
	assert(randomInitialized);
	return lowerLimit + (rand() % upperLimit);
}

float randomf() {
	assert(randomInitialized);
	unsigned int bits = rand();
	float f = *(float *)&bits;
	return f;
}
