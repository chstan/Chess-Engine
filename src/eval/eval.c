#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../move/movegen.h"
#include "eval.h"
#include "evalhelpers.h"
#include "../defines.h"

float evaluate(Board *pBoard) {
    // for now we just use a naive evaluation, which counts material and does
    // somewhat primitive checks of mobility and board control
    return evaluateNaive(pBoard);
}

float evaluateNaive(Board *pBoard) {
    // obviously a bad evaluation function, but it's a start to test the search functions and the cohesion of the engine
    //float materialDelta = (float) log((float) pBoard->info.whiteMaterial/pBoard->info.blackMaterial);
    if(checks(pBoard, otherColor(pBoard->info.toPlay))) {
        return pBoard->info.toPlay == W ? INFTY : -INFTY;
    }
    float materialDelta = (float) pBoard->info.whiteMaterial - pBoard->info.blackMaterial;

    float value = materialDelta; // + (-0.01 * doubledPawns(pBoard)) + (-0.02 * tripledPawns(pBoard)) + (0.05 * passedPawns(pBoard)) + (0.03 * outsidePassedPawns(pBoard));
    return (pBoard->info.toPlay == W) ? value : -value;
}
