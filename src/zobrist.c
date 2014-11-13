#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "zobrist.h"
#include "move/move.h"

U64 piece_keys[16][64];

U64 castling_rights_keys[16];

U64 white_to_move_key;

U64 ep_file_keys[8];

void realloc_hash_mem();

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

    realloc_hash_mem();
}

// ================= TRANSPOSITION TABLE ===================

unsigned long long filled_hash_elems = 0;
unsigned int hash_time = 0;

unsigned char PV_NODE = 0;
unsigned char FAIL_LOW_NODE = 1;
unsigned char FAIL_HIGH_NODE = 2;

bool is_pv_node(TTElem *e) {
    return e && PV_NODE == node_type(e->_flags);
}
bool is_fail_high(TTElem *e) {
    return e && FAIL_HIGH_NODE == node_type(e->_flags);
}
bool is_fail_low(TTElem *e) {
    return e && FAIL_LOW_NODE == node_type(e->_flags);
}

void inc_hash_time() {
    hash_time++;
    hash_time %= (1 << 6);
}

void set_current_hash_time(unsigned int time) {
    hash_time = time;
}

unsigned char insert_time(unsigned char flags) {
    return flags % (1 << 6);
}

unsigned char node_type(unsigned char flags) {
    return flags >> 6;
}

unsigned char build_flags(unsigned char node_type, unsigned char insert_time) {
    return (node_type << 6) | insert_time;
}

unsigned int log_total_elems = 21;
unsigned int log_num_buckets = 19;
unsigned int elems_per_bucket;
unsigned int num_buckets;

TTElem *hash_mem = NULL;

TTElem *lookup_bucket_by_key(U64 key) {
    // because there are num_buckets number of buckets
    // we can get a bitmask for the bucket number by
    unsigned int which_bucket = key & (num_buckets - 1);
    return hash_mem + 4*(which_bucket);
}

void write_hash(U64 key, int score, Move m, unsigned char depth,
                unsigned char n_type) {
    //setHashBit(m);
    TTElem *first_in_bucket = lookup_bucket_by_key(key);
    TTElem *b_iter = first_in_bucket;
    for (size_t elem_iter = 0; elem_iter < elems_per_bucket;
         elem_iter++, b_iter++) {
        if (!b_iter->_key || b_iter->_key == key) {
            if (!b_iter->_key) {
                filled_hash_elems++;
            }
            b_iter->_key = key;
            b_iter->_score = score;
            b_iter->_m = m;
            b_iter->_depth = depth;
            b_iter->_flags = build_flags(n_type, hash_time);
            return;
        }
    }

    // didn't find an easy to replace node, instead go ahead
    // and replace
    b_iter = first_in_bucket;
    char worst_score = 0;
    TTElem *to_replace = first_in_bucket;
    for (size_t elem_iter = 0; elem_iter < elems_per_bucket;
         elem_iter++, b_iter++) {
        char elem_score = 0;
        if (insert_time(b_iter->_flags) == hash_time ||
            node_type(b_iter->_flags) == PV_NODE) elem_score++;

        if (insert_time(to_replace->_flags) == hash_time) elem_score--;

        if (b_iter->_depth < to_replace->_depth) elem_score--;

        if (elem_score < worst_score) {
            worst_score = elem_score;
            to_replace = b_iter;
        }
    }
    to_replace->_key = key;
    to_replace->_score = score;
    to_replace->_m = m;
    to_replace->_depth = depth;
    to_replace->_flags = build_flags(n_type, hash_time);
}

void update_time(TTElem *to_update) {
    to_update->_flags = build_flags(node_type(to_update->_flags), hash_time);
}

TTElem *search_hash(U64 key) {
    TTElem *bucket = lookup_bucket_by_key(key);
    for (size_t elem_iter = 0; elem_iter < elems_per_bucket; ++elem_iter, ++bucket) {
        if (bucket->_key == key) {
            update_time(bucket);
            return bucket;
        }
    }
    return NULL;
}

void free_hash() {
    if (hash_mem) free(hash_mem);
}

void reset_hash() {
    filled_hash_elems = 0;
    memset(hash_mem, 0, elems_per_bucket * num_buckets * sizeof(TTElem));
}

unsigned long long hash_per_million_full() {
    return (filled_hash_elems * 1000000) /
        ((unsigned long long) (elems_per_bucket * num_buckets));
}

void realloc_hash_mem() {
    num_buckets = 1 << log_num_buckets;
    elems_per_bucket = 1 << (log_total_elems - log_num_buckets);
    if (hash_mem) {
        free(hash_mem);
        hash_mem = NULL;
    }
    hash_mem = malloc(elems_per_bucket * num_buckets * sizeof(TTElem));
    assert(hash_mem != NULL);
    reset_hash();
}
