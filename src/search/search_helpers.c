#include "search_helpers.h"
#include "../manage_time.h"

void sync_to_TT(U64 key, TTElem *in_table, Move move, int value,
                int alpha, int beta, int depth, bool check_for_stop) {
    // only sync if safe to do so
    if (check_for_stop && should_stop()) return;

    unsigned char node_type = (value <= alpha) ?
        FAIL_LOW_NODE : (value >= beta) ?
        FAIL_HIGH_NODE : PV_NODE;
    // this is an interesting question
    // when do we update the table? It would make sense to update
    // only when we are doing searches to greater depth, I think
    // at least with the present search algorithm, which is deterministic
    if ((in_table && depth > in_table->_depth) || !in_table)
        write_hash(key, value, move, depth, node_type);
}
