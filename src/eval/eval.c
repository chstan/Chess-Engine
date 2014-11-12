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
    pawn_structure_score -= doubledPawns(pBoard);
    pawn_structure_score -= 3*tripledPawns(pBoard);
    pawn_structure_score += 5*passedPawns(pBoard);
    pawn_structure_score += 3*outsidePassedPawns(pBoard);
    value += pawn_structure_score;
    return (pBoard->info.toPlay == W) ? value : -value;
}
