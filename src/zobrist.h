#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "defines.h"

typedef unsigned int Move;

/* Function initHashTable
 * ----------------------
 * allocates random strings for board position hashing
 * as well as the hash table itself used to store move positions
 * and evaluations
 */
void free_hash();
void initHashTable();

// keys for each piece at each board position
// for simplicity we use 16 pieces although there are
// only 12 used, some of the piece indices are not in use.
extern U64 piece_keys[16][64];

// castling rights
extern U64 castling_rights_keys[16];

extern U64 white_to_move_key;

extern U64 ep_file_keys[8];

extern unsigned char PV_NODE;
extern unsigned char FAIL_LOW_NODE;
extern unsigned char FAIL_HIGH_NODE;

// ================== DATA TYPES ===================

typedef struct {
    U64 _key;
    int _score;
    Move _m;
    unsigned char _depth;
    // | two bits node type | 6 bits insert_time |
    unsigned char _flags;
} TTElem;

// other transposition table stuff

bool is_pv_node(TTElem *e);
bool is_fail_high(TTElem *e);
bool is_fail_low(TTElem *e);

unsigned long long hash_per_million_full();

void reset_hash();
void inc_hash_time();
void set_current_hash_time(unsigned int time);
unsigned char insert_time(unsigned char flags);
unsigned char node_type(unsigned char flags);
TTElem *search_hash(U64 key);
void write_hash(U64 key, int score, Move m, unsigned char depth,
                   unsigned char node_type);

#endif
