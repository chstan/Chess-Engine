#ifndef SEARCH_HELPERS_H
#define SEARCH_HELPERS_H

#include "../defines.h"
#include "../zobrist.h"

void sync_to_TT(U64 key, TTElem *in_table, Move move,
                int value, int alpha, int beta, int depth, bool check_for_stop);

#endif
