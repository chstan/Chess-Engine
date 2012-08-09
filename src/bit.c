#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bit.h"
#include "defines.h"
#include "extglobals.h"

unsigned int countBits(BitMap bits) {
	unsigned int bitCount;
	// each loop reduces the number of set bits by one
	for(bitCount = 0; bits; bitCount++) {
		bits &= (bits - 1);
	}
	return bitCount;
}

unsigned int LSB(BitMap bits) {
	if(!bits) return 0;
	unsigned long long lsb;	
	#ifdef __clang__
	asm("bsfq %1, %0" : "=r" (lsb) : "r" (bits));
	#elif (__GNUC__ || __cplusplus)
	__asm__("bsfq %1, %0" : "=r" (lsb) : "r" (bits));
	#endif
	return lsb;
}

unsigned int GSB(BitMap bits) {
	if(!bits) return 0;
	unsigned long long gsb;	
	#ifdef __clang__
	asm("bsrq %0, %1" : "=r" (gsb) : "r" (bits));
	#elif (__GNUC__ || __cplusplus)
	__asm__("bsrq %0, %1" : "=r" (gsb) : "r" (bits));
	#endif
	return gsb;
}

void printBitMap(BitMap bits) {
	int rank, file;
	for(rank = 8; rank >= 1; rank--) {
		for(file = 1; file <= 8; file++) {
			printf(" %s ", (bits & BITSET(sq(rank, file))) ? "#" : ".");
		}
		printf("\n");
	}
	printf("\n");
	return;
}
