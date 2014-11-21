#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../move/movegen.h"
#include "eval.h"
#include "evalhelpers.h"
#include "../defines.h"

const int EVAL_INFTY = 9999999;
const int EVAL_MATE = EVAL_INFTY - 10000;

int evaluate(Board *pBoard) {
    // for now we just use a naive evaluation, which counts material and does
    // somewhat primitive checks of mobility and board control
    return evaluateNaive(pBoard);
}

int evaluateNaive(Board *pBoard) {
    // obviously a bad evaluation function, but it's a
    // start to test the search functions and the cohesion of the engine
    int materialDelta = pBoard->info.whiteMaterial - pBoard->info.blackMaterial;
    int value = materialDelta;
    int pawn_structure_score = 0;


    BitBoard white_pawn_forward_range;
    BitBoard white_pawn_backward_range;
    BitBoard black_pawn_forward_range;
    BitBoard black_pawn_backward_range;
    __attribute__((unused)) BitBoard white_pawn_wide_forward_range;
    __attribute__((unused)) BitBoard black_pawn_wide_forward_range;

    build_pawn_ranges(pBoard,
                      &white_pawn_forward_range,
                      &white_pawn_backward_range,
                      &black_pawn_forward_range,
                      &black_pawn_backward_range,
                      &white_pawn_wide_forward_range,
                      &black_pawn_wide_forward_range);

    pawn_structure_score -= doubledPawns(pBoard);
    pawn_structure_score -= 3*tripledPawns(pBoard);
    pawn_structure_score += 5*passed_pawns(pBoard,
                                           white_pawn_forward_range,
                                           black_pawn_forward_range);
    // this needs to be considered in more depth. What constitutes
    // an outside passer?
    //pawn_structure_score +=
    //    3*outside_passed_pawns(pBoard,
    //                           white_pawn_wide_forward_range,
    //                           black_pawn_wide_forward_range);

    value += pawn_structure_score;
    value += all_piece_square_scores(pBoard);
    return (pBoard->info.toPlay == W) ? value : -value;
}
