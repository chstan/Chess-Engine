#ifndef BIT_H
#define BIT_H

#include "defines.h"

/* Function: countBits
 * -------------------
 * countBits uses the Kernighan's
 * bit counting method to determine
 * the number of bits set in a long long (BitMap)
 */
unsigned int countBits(BitMap bits);

/* Function: LSB
 * -------------
 * Uses DeBruijin's multiplication to find the
 * least significant bit which is set to 1 in a 
 * BitMap. This function looks like magic because it is.
 */
unsigned int LSB(BitMap bits);

/* Function: GSB
 * -------------
 * Like the above function, it finds the greatest significant bit
 * set to 1 in the bitmap by the naive method. It uses more memory
 * and is slower than the LSB method. I'm sure someone could do a bit
 * of research and find a magic number that would work for
 * DeBruijin multiplication here too.
 */
unsigned int GSB(BitMap bits);

/* Function: printBitMap
 * ---------------------
 * Pretty prints the bitmap in an
 * 8x8 square with the least significant bit
 * at the bottom left, and the next bit to
 * its right.
 *				8 9 ...
 *				0 1 2 3 4 5 6 7
 */
void printBitMap(BitMap bits);

#endif
