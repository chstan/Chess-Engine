#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "search.h"
#include "search_helpers.h"
#include "../log.h"
#include "../manage_time.h"
#include "../zobrist.h"
#include "../move/movegen.h"
#include "../move/notation.h"
#include "../eval/eval.h"
#include "../board/board.h"
#include "../defines.h"
#include "../extglobals.h"
#include "../uci.h"

// kind of a hack, would be better if we had
// proper data structures, vectors or stacks would be nice
// C++!
const int search_depth = 60;

Move get_next_pv(Board *p_board) {
    MoveSet moves;
    reset_move_set(&moves);
    initialize_move_set_quiet(p_board, &moves);

    U64 key = p_board->info.state[p_board->info.current_move]._zobrist_key;
    TTElem *table_entry = search_hash(key);
    if (!table_entry) return 0;
    if (!moveset_contains(&moves, table_entry->_m)) {
        // Statistically this doesn't happen, but just to be safe
        return 0;
    }
    return table_entry->_m;
}

void print_pv(Board *p_board, int depth, int score, Move root_best_move,
              size_t searched_nodes, clock_t searched_ticks) {
    Move moves[MAX_MOVES_PER_GAME];
    U64 keys[MAX_MOVES_PER_GAME];
    int iter = 0;
    //U64 orig_key = p_board->info.state[p_board->info.current_move]._zobrist_key;
    for(Move best_move = root_best_move; best_move;
        best_move = get_next_pv(p_board)) {
        if(!best_move) {
            break;
        }
        bool is_rep = false;
        for (int k = 0; k < iter; k++) {
            if (p_board->info.state[p_board->info.current_move]._zobrist_key == keys[k]) {
                is_rep = true;
                break;
            }
        }
        if (is_rep) break;

        moves[iter] = best_move;
        keys[iter] = p_board->info.state[p_board->info.current_move]._zobrist_key;
        iter++;
        make_move(p_board, best_move);
    }
    int n_moves = iter;
    iter--;
    for (; iter >= 0; iter--) {
        unmake_move(p_board, moves[iter]);
    }

    size_t searched_ms = (((unsigned long long int) searched_ticks) * 1000) / CLOCKS_PER_SEC;
    uci_info(depth, 0, score, 0, searched_nodes, searched_ms, moves, n_moves);
}

void *threadable_think(void *arg) {
    log_string("Starting to think.\n");
    Board *p_board = (Board *) arg;
    Move m = think(p_board);
    uci_best_move(m);
    set_is_thinking(false);
    log_string("Ending think.\n");
    return NULL;
}

Move think(Board *p_board) {
    // a really stupid search for the moment
    // should manage its time and use an iterative deepening method

    Move best_move_last = 0;
    Move best_move = 0;

    size_t searched_nodes = 0;
    clock_t total_search_ticks = 0;

    int color = p_board->info.to_play == W ? 1 : -1;
    int score;

    start_search_clock(p_board->info.to_play == W);

    clock_t ticks_start = clock();
    score = nega_max(0, 1, -EVAL_INFTY, EVAL_INFTY, color, &best_move,
                    &searched_nodes, false);
    inc_hash_time();
    clock_t ticks_end = clock();

    total_search_ticks += (ticks_end - ticks_start);

    best_move_last = best_move;

    for (int c_depth = 2; c_depth <= search_depth &&
             should_continue_greater_depth(ticks_end - ticks_start); c_depth++) {
        ticks_start = ticks_end;
        score = nega_max(0, c_depth, -EVAL_INFTY, EVAL_INFTY,
                        color, &best_move, &searched_nodes, true);
        inc_hash_time();
        ticks_end = clock();
        total_search_ticks += (ticks_end - ticks_start);

        if (should_stop()) {
            best_move = best_move_last;
        } else {
            best_move_last = best_move;
            if (!best_move) {
                log_printf("Game over!\n");
                break;
            }
            print_pv(p_board, c_depth, score, best_move, searched_nodes, total_search_ticks);
        }
        if (!best_move) {
            log_printf("Game over!\n");
            break;
        }
    }
    return best_move;
}

int quiescent_nega_max(int ply, int alpha, int beta, int color, Move *pm,
                     size_t *searched_nodes) {
    (*searched_nodes)++;
    int base_value = evaluate(p_board);

    MoveSet moves;
    int value;

    reset_move_set(&moves);
    generate_capture(p_board, &moves);
    qsort(&(moves.move_list[0]), moves.total_moves, sizeof(Move), comp_move);

    if (base_value >= beta) {
        //for (int i = 0; i < moves.total_moves; i++) {
        //    make_move(p_board, moves.move_list[i]);
        //    if (checks(p_board, other_color(p_board->info.to_play))) {
        //        unmake_move(p_board, moves.move_list[i]);
        //        continue;
        //    } else {
        //        unmake_move(p_board, moves.move_list[i]);
        //        (*pm) = moves.move_list[i];
        //        return base_value;
        //    }
        //}
        return base_value;
    }
    if (alpha < base_value) alpha = base_value;

    int legal_moves = 0;
    for (int i = 0; i < moves.total_moves; i++) {
        make_move(p_board, moves.move_list[i]);
        if (checks(p_board, other_color(p_board->info.to_play))) {
            unmake_move(p_board, moves.move_list[i]);
            continue;
        }
        // get a legal move to return worst case,
        // represents a search failure because each move is
        // worst than the static evaluation
        if (!*pm) (*pm) = moves.move_list[i];

        legal_moves++;
        Move best_quiesce = 0; // unused
        value = -quiescent_nega_max(ply + 1, -beta, -alpha, -1*color, &best_quiesce,
                                  searched_nodes);
        unmake_move(p_board, moves.move_list[i]);

        if (value >= beta) return beta;
        if (value > alpha) {
            alpha = value;
        }
    }
    if (!legal_moves) {
        (*pm) = 0; // no legal move to return
        if (checks(p_board, p_board->info.to_play)) {
            return -EVAL_MATE + ply; // mated
        } else {
            return 0; // draw
        }
    }
    return alpha;
}

// color is +1 for white and -1 for black, reflecting the consistency that
// for the purposes of board evaluation positive scores favor the person
// to play
int nega_max(int ply, int depth, int alpha, int beta, int color, Move *pm,
            size_t *searched_nodes, bool check_for_stop) {
    (*searched_nodes)++;
    int value = -EVAL_INFTY;
    int bestValue = -EVAL_INFTY;
    MoveSet moves;
    int alpha_original = alpha;

    if (check_for_stop && should_stop()) {
        return bestValue;
    }

    if (depth == 0) {
        Move best_quiesce_move = 0;
        value = quiescent_nega_max(ply, alpha, beta, color, &best_quiesce_move,
                                 searched_nodes);
        (*pm) = best_quiesce_move;
        return value;
    }

    // need to be more careful about whether the stored move is legal
    (*pm) = 0;

    U64 zob_key = p_board->info.state[p_board->info.current_move]._zobrist_key;
    TTElem *table_entry = search_hash(zob_key);
    if (table_entry && table_entry->_depth >= depth) {
        (*pm) = table_entry->_m;
        if (is_pv_node(table_entry)) {
            return table_entry->_score;
        } else if (is_fail_high(table_entry) && table_entry->_score > alpha) {
            alpha = table_entry->_score;
        } else if (is_fail_low(table_entry) && table_entry->_score < beta) {
            beta = table_entry->_score;
        }
        if (alpha >= beta) {
            return table_entry->_score;
        }
    }

    Move best_found_move = table_entry ? table_entry->_m : 0;
    (*pm) = best_found_move;

    reset_move_set(&moves);
    initialize_move_set_quiet(p_board, &moves);
    qsort(&(moves.move_list[0]), moves.total_moves, sizeof(Move), comp_move);

    int legal_moves = 0;
    if (table_entry && moveset_contains(&moves, table_entry->_m)) {
        make_move(p_board, table_entry->_m);
        // checks? if (checks(p_board, other_color(p_board->info.to_play))) {...}
        // not needed since is in the transposition table
        Move t = 0;
        value = -nega_max(ply + 1, depth - 1, -beta, -alpha, -color, &t,
                         searched_nodes, check_for_stop);
        unmake_last_move(p_board);
        bestValue = value;
        if (value > alpha) {
            // this is already set
            // best_found_move = table_entry->_m;
            alpha = value;
        }
        if (alpha >= beta) {
            // fail
            return value;
        }
    } else {
        // actually referred to a different position
        table_entry = NULL;
    }

    for (int i = 0; i < moves.total_moves; i++) {
        // no need to retry TT best move
        if (table_entry && table_entry->_m == moves.move_list[i]) {
            legal_moves++;
            continue;
        }

        make_move(p_board, moves.move_list[i]);
        if (checks(p_board, other_color(p_board->info.to_play))) {
            unmake_move(p_board, moves.move_list[i]);
            continue;
        }
        legal_moves++;
        if (!best_found_move) {
            best_found_move = moves.move_list[i];
            (*pm) = best_found_move;
        }
        Move t = 0;
        value = -nega_max(ply+1, depth-1, -beta, -alpha, -1 * color, &t,
                         searched_nodes, check_for_stop);
        unmake_move(p_board, moves.move_list[i]);

        if (value > bestValue) bestValue = value;
        if (value > alpha) {
            best_found_move = moves.move_list[i];
            (*pm) = best_found_move;
            alpha = value;
        }
        if (alpha >= beta)
            break;
    }
    if (legal_moves == 0) {
        if (checks(p_board, p_board->info.to_play)) {
            bestValue = -EVAL_MATE + ply;
        } else {
            bestValue = 0;
        }
    }

    // synchronize to TT, handles issue of when its safe to sync
    sync_to_TT(zob_key, table_entry, best_found_move, bestValue, alpha_original, beta,
               depth, check_for_stop);

    return bestValue;
}
