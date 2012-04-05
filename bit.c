#include "defines.h"
#include "extglobals.h"
#include <assert.h>

unsigned int countBits(BitMap bits) {
	unsigned int bitCount;
	for(bitCount = 0; bits; bitCount++) {
		bits &= bits - 1;
	}
	return bitCount;
}

unsigned int LSB(BitMap bits) {
	assert(bits != 0);
	
	// This uses De Bruijin Multiplication
	static const int INDEX[64] = {
		63,  0, 58,  1, 59, 47, 53,  2,
		60, 39, 48, 27, 54, 33, 42,  3,
		61, 51, 37, 40, 49, 18, 28, 20,
		55, 30, 34, 11, 43, 14, 22,  4,
		62, 57, 46, 52, 38, 26, 32, 41,
		50, 36, 17, 19, 29, 10, 13, 21,
		56, 45, 25, 31, 35, 16,  9, 12,
		44, 24, 15,  8, 23,  7,  6,  5
	};
	static const U64 DeBruijn = 0x07EDD5E59A4E28C2;
	
	return INDEX[((bits & -bits) * DeBruijn) >> 58];
}

unsigned int GSB(BitMap bits) {
	assert(bits != 0);
	
	int GSB = 0;
	if(bits > 0xFFFFFFFF) {
		bits >>= 32;
		GSB = 32;
	}
	if(bits > 0xFFFF) {
		bits >>= 16;
		GSB += 16;
	}
	if(bits > 0xFF) {
		bits >>= 8;
		GSB += 8;
	}
	//lookup the last byte in a table
	return GSB + GS1B[bits];
}

