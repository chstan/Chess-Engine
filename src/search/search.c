#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "search.h"
#include "../zobrist.h"
#include "../move/movegen.h"
#include "../move/notation.h"
#include "../eval/eval.h"
#include "../board/board.h"
#include "../defines.h"
#include "../extglobals.h"

// kind of a hack, would be better if we had
// proper data structures, vectors or stacks would be nice
// C++!
const int search_depth = 60;

Move singleBestMove;

Move get_next_pv(Board *pBoard) {
    U64 key = pBoard->info.state[pBoard->info.currentMove]._zobrist_key;
    TTElem *lookup = search_hash(key);
    if (!lookup) return 0;
    return lookup->_m;
}

void print_pv(Board *pBoard) {
    Move moves[64];
    int iter = 0;
    char *notation = NULL;
    U64 orig_key = pBoard->info.state[pBoard->info.currentMove]._zobrist_key;
    for(Move best_move = singleBestMove; best_move;
        best_move = get_next_pv(pBoard)) {
        moves[iter] = best_move;
        iter++;
        notation = moveToNotation(pBoard, best_move);
        assert(notation);
        printf("%s ", notation);
        free(notation);
        notation = NULL;
        makeMove(pBoard, best_move);
    }
    iter--;
    printf("\b\n");
    for (; iter >= 0; iter--) {
        unmakeMove(pBoard, moves[iter]);
    }
}

Move think(Board *pBoard) {
    // a really stupid search for the moment
    // should manage its time and use an iterative deepening method
    clock_t end_time = clock() + (10 * CLOCKS_PER_SEC);
    int color = pBoard->info.toPlay == W ? 1 : -1;
    float score = -INFTY;
    for (int c_depth = 2; c_depth <= search_depth; c_depth++) {
        if (clock() > end_time) break;
        inc_hash_time();
        printf("Depth: %d\n", c_depth);
        //print_pv(pBoard);

        score = negaMax(0, c_depth,
                        (float) -INFTY,
                        (float) INFTY, color, &singleBestMove);
    }

    assert(singleBestMove);
    printf("info score cp 0 depth 6 move: %s\n", move_to_coord_notation(pBoard, singleBestMove));
    return singleBestMove;
}

float quiescentNegaMax(float alpha, float beta, int color, Move *pm) {
    float base_value = evaluate(pBoard);

    MoveSet moves;
    float value;

    resetMoveSet(&moves);
    generateCapture(pBoard, &moves);

    if (base_value >= beta) {
        //for (int i = 0; i < moves.totalMoves; i++) {
        //    makeMove(pBoard, moves.moveList[i]);
        //    if (checks(pBoard, otherColor(pBoard->info.toPlay))) {
        //        unmakeMove(pBoard, moves.moveList[i]);
        //        continue;
        //    } else {
        //        unmakeMove(pBoard, moves.moveList[i]);
        //        (*pm) = moves.moveList[i];
        //        return base_value;
        //    }
        //}
        return base_value;
    }
    if (alpha < base_value) alpha = base_value;

    bool legal_moves = false;
    for (int i = 0; i < moves.totalMoves; i++) {
        makeMove(pBoard, moves.moveList[i]);
        if (checks(pBoard, otherColor(pBoard->info.toPlay))) {
            unmakeMove(pBoard, moves.moveList[i]);
            continue;
        }
        // get a legal move to return worst case,
        // represents a search failure because each move is
        // worst than the static evaluation
        if (!*pm) (*pm) = moves.moveList[i];

        legal_moves = true;
        Move best_quiesce = 0; // unused
        value = -quiescentNegaMax(-beta, -alpha, -1*color, &best_quiesce);
        unmakeMove(pBoard, moves.moveList[i]);

        if (value >= beta) return beta;
        if (value > alpha) {
            alpha = value;
        }
    }
    if (!legal_moves) {
        (*pm) = 0; // no legal move to return
        return -INFTY;
    }

    return alpha;
}

// color is +1 for white and -1 for black, reflecting the consistency that
// for the purposes of board evaluation positive scores favor the person
// to play
float negaMax(int ply, int depth, float alpha, float beta, int color, Move *pm) {
    float value = -INFTY;
    float bestValue = -INFTY;
    MoveSet moves;

    // need to be more careful about whether the stored move is legal
    (*pm) = 0;
    U64 zob_key = pBoard->info.state[pBoard->info.currentMove]._zobrist_key;
    TTElem *table_entry = search_hash(zob_key);
    if (table_entry && table_entry->_depth >= depth) {
        if (is_pv_node(table_entry)) {
            (*pm) = table_entry->_m;
            return table_entry->_score;
        } else if (is_fail_high(table_entry) && table_entry->_score > beta) {
            beta = table_entry->_score;
        } else if (is_fail_low(table_entry) && table_entry->_score < alpha) {
            alpha = table_entry->_score;
        }
        if (alpha >= beta) {
            return value;
        }
    }

    if (depth == 0) {
        Move best_quiesce_move = 0;
        value = quiescentNegaMax(alpha, beta, color, &best_quiesce_move);
        if (!best_quiesce_move)
            return value;
        if (value <= alpha) {
            write_hash(zob_key, value, best_quiesce_move, depth, FAIL_LOW_NODE);
        } else if (value >= beta) {
            write_hash(zob_key, value, best_quiesce_move, depth, FAIL_HIGH_NODE);
        } else {
            write_hash(zob_key, value, best_quiesce_move, depth, PV_NODE);
        }
        (*pm) = best_quiesce_move;
        return value;
    }

    Move best_found_move = table_entry ? table_entry->_m : 0; // principal variation
    (*pm) = best_found_move;

    resetMoveSet(&moves);
    initializeMoveSetQuiet(pBoard, &moves);
    qsort(&moves.moveList, moves.totalMoves, sizeof(Move), compMove);

    if (table_entry) {
        if(!moveset_contains(&moves, table_entry->_m)) {
            return bestValue;
        }

        makeMove(pBoard, table_entry->_m);
        // checks? if (checks(pBoard, otherColor(pBoard->info.toPlay))) {...}
        // not needed since is in the transposition table
        Move t = 0;
        value = -negaMax(ply + 1, depth - 1, -beta, -alpha, -1 * color, &t);
        unmakeMove(pBoard, table_entry->_m);
        if (value > bestValue) {
            bestValue = value;
        }
        if (value > alpha) {
            // this is already set
            // best_found_move = table_entry->_m;
            alpha = value;
        }
        if (alpha >= beta) {
            // fail
            moves.totalMoves = 0;
        }
    }

    for (int i = 0; i < moves.totalMoves; i++) {
        // no need to retry TT best move
        if (table_entry && table_entry->_m == moves.moveList[i])
            continue;

        makeMove(pBoard, moves.moveList[i]);
        if (checks(pBoard, otherColor(pBoard->info.toPlay))) {
            unmakeMove(pBoard, moves.moveList[i]);
            continue;
        }
        if (!best_found_move) {
            best_found_move = moves.moveList[i];
            (*pm) = best_found_move;
        }
        Move t = 0;
        value = -negaMax(ply+1, depth-1, -beta, -alpha, -1 * color, &t);
        unmakeMove(pBoard, moves.moveList[i]);

        if (value > bestValue) bestValue = value;
        if (value > alpha) {
            best_found_move = moves.moveList[i];
            (*pm) = best_found_move;
            if (ply == 0) {
                singleBestMove = moves.moveList[i];
            }
            alpha = value;
        }
        if (alpha >= beta)
            break;
    }

    // synchronize to TT
    if (bestValue <= alpha) {
        write_hash(zob_key, bestValue, best_found_move, depth, FAIL_LOW_NODE);
    } else if (bestValue >= beta) {
        write_hash(zob_key, bestValue, best_found_move, depth, FAIL_HIGH_NODE);
    } else {
        write_hash(zob_key, bestValue, best_found_move, depth, PV_NODE);
    }
    return bestValue;
}
