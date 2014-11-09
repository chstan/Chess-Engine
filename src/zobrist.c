#include <stdlib.h>

#include "zobrist.h"

U64 piece_keys[16][64];

U64 castling_rights_keys[16];

U64 white_to_move_key;

U64 ep_file_keys[8];

void initHashTable() {
    U64 temp;

    for (unsigned int piece_idx = 0; piece_idx < 16; piece_idx++) {
        temp = rand();
        castling_rights_keys[piece_idx] = (temp << 32) | rand();
        for (unsigned int sq_idx = 0; sq_idx < 64; sq_idx++) {
            temp = rand();
            piece_keys[piece_idx][sq_idx] = (temp << 32) | rand();
        }
    }

    temp = rand();
    white_to_move_key = (temp << 32) | rand();

    for (unsigned int file_idx = 0; file_idx < 8; file_idx++) {
        temp = rand();
        ep_file_keys[file_idx] = (temp << 32) | rand();
    }
}

// ================= TRANSPOSITION TABLE ===================

int elems_per_bucket = 1 << 2;
int num_buckets = 1 << 18;

//TTElem **buckets;
