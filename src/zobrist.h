#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "defines.h"

/* Function initHashTable
 * ----------------------
 * allocates random strings for board position hashing
 * as well as the hash table itself used to store move positions
 * and evaluations
 */
void initHashTable();

// keys for each piece at each board position
// for simplicity we use 16 pieces although there are
// only 12 used, some of the piece indices are not in use.
U64 piece_keys[16][64];

// castling rights
U64 castling_rights_keys[16];

U64 white_to_move_key;

U64 ep_file_keys[8];

#endif
