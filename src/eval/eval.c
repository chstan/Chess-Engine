#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "eval.h"
#include "../zobrist.h"
#include "../move/movegen.h"
#include "evalhelpers.h"
#include "../defines.h"

const int EVAL_INFTY = 9999999;
const int EVAL_MATE = EVAL_INFTY - 10000;

int evaluate(Board *p_board) {
    // for now we just use a naive evaluation, which counts material and does
    // somewhat primitive checks of mobility and board control

    // check for position in the eval hash table, write new entry if necessary
    U64 key = p_board->info.state[p_board->info.current_move]._zobrist_key;
    EvalTElem *eval_table_elem = search_eval_hash(key);
    if (eval_table_elem)
        return eval_table_elem->_score;

    int evaluation = evaluate_naive(p_board);

    write_eval_hash(key, evaluation);
    return evaluation;
}

int evaluate_naive(Board *p_board) {
    // obviously a bad evaluation function, but it's a
    // start to test the search functions and the cohesion of the engine
    int materialDelta = p_board->info.white_material - p_board->info.black_material;
    int value = materialDelta;
    int pawn_structure_score = 0;


    U64 white_pawn_forward_range;
    U64 white_pawn_backward_range;
    U64 black_pawn_forward_range;
    U64 black_pawn_backward_range;
    __attribute__((unused)) U64 white_pawn_wide_forward_range;
    __attribute__((unused)) U64 black_pawn_wide_forward_range;

    build_pawn_ranges(p_board,
                      &white_pawn_forward_range,
                      &white_pawn_backward_range,
                      &black_pawn_forward_range,
                      &black_pawn_backward_range,
                      &white_pawn_wide_forward_range,
                      &black_pawn_wide_forward_range);

    pawn_structure_score -= doubled_pawns(p_board);
    pawn_structure_score -= 3*tripled_pawns(p_board);
    pawn_structure_score += 5*passed_pawns(p_board,
                                           white_pawn_forward_range,
                                           black_pawn_forward_range);
    // this needs to be considered in more depth. What constitutes
    // an outside passer?
    //pawn_structure_score +=
    //    3*outside_passed_pawns(p_board,
    //                           white_pawn_wide_forward_range,
    //                           black_pawn_wide_forward_range);

    value += pawn_structure_score;
    value += all_piece_square_scores(p_board);
    return (p_board->info.to_play == W) ? value : -value;
}
