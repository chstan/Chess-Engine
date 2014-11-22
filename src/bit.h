#ifndef BIT_H
#define BIT_H

#include "defines.h"

/* Function: count_bits
 * -------------------
 * count_bits uses the Kernighan's
 * bit counting method to determine
 * the number of bits set in a long long (U64)
 */
unsigned int count_bits(U64 bits);

/* Function: LSB
 * -------------
 * Uses DeBruijin's multiplication to find the
 * least significant bit which is set to 1 in a
 * U64. This function looks like magic because it is.
 */
unsigned int LSB(U64 bits);

/* Function: GSB
 * -------------
 * Like the above function, it finds the greatest significant bit
 * set to 1 in the bitmap by the naive method. It uses more memory
 * and is slower than the LSB method. I'm sure someone could do a bit
 * of research and find a magic number that would work for
 * DeBruijin multiplication here too.
 */
unsigned int GSB(U64 bits);

/* Function: print_U64
 * ---------------------
 * Pretty prints the bitmap in an
 * 8x8 square with the least significant bit
 * at the bottom left, and the next bit to
 * its right.
 *              8 9 ...
 *              0 1 2 3 4 5 6 7
 */
void print_U64(U64 bits);

/** Function: shared_rank
 *  --------------------
 * Determines whether all the on bits in a bitboard fall
 * in the same rank.
**/
bool shared_rank(U64 bits);

/** Function: shared_file
 *  --------------------
 * Determines whether all the on bits in a bitboard fall
 * in the same file.
**/
bool shared_file(U64 bits);

#endif
