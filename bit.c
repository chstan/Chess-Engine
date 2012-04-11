#include "defines.h"
#include "extglobals.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Function: countBits
 * -------------------
 * countBits uses the Kernighan's
 * bit counting method to determine
 * the number of bits set in a long long (BitMap)
 */
unsigned int countBits(BitMap bits) {
	unsigned int bitCount;
	// each loop reduces the number of set bits by one
	for(bitCount = 0; bits; bitCount++) {
		bits &= (bits - 1);
	}
	return bitCount;
}

/* Function: LSB
 * -------------
 * Uses DeBruijin's multiplication to find the
 * least significant bit which is set to 1 in a 
 * BitMap. This function looks like magic because it is.
 */
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
	/* This magic number multiplies so that the least significant bit
	 * sets bits [58-64] in the result. This is why we shift over 
	 * by fifty-eight bits before we look into the array.
	 */
	static const U64 DeBruijn = 0x07EDD5E59A4E28C2;
	
	return INDEX[((bits & -bits) * DeBruijn) >> 58];
}

/* Function: GSB
 * -------------
 * Like the above function, it finds the greatest significant bit
 * set to 1 in the bitmap by the naive method. It uses more memory
 * and is slower than the LSB method. I'm sure someone could do a bit
 * of research and find a magic number that would work for
 * DeBruijin multiplication here too.
 */
unsigned int GSB(BitMap bits) {
	assert(bits != 0);
	
	int GSB = 0;
	// Is the GSB at least 32?
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

/* Function: printBitMap
 * ---------------------
 * Pretty prints the bitmap in an
 * 8x8 square with the least significant bit
 * at the bottom left, and the next bit to
 * its right.
 *				8 9 ...
 *				0 1 2 3 4 5 6 7
 */
void printBitMap(BitMap bits) {
	int rank, file;
	for(rank = 8; rank >= 1; rank--) {
		for(file = 1; file <= 8; file++) {
			printf(" %s ", (bits & BITSET[sq(rank, file)]) ? "#" : ".");
		}
		printf("\n");
	}
	printf("\n");
	return;
}
