#include <stdio.h>
#include <stdlib.h>

#include "search.h"
#include "../move/movegen.h"
#include "../eval/eval.h"
#include "../board/board.h"
#include "../defines.h"
#include "../extglobals.h"

// kind of a hack, would be better if we had
// proper data structures, vectors or stacks would be nice
// C++!
const int quiescent_search_depth = 3;
const int search_depth = 4;

Move singleBestMove;
int bestMoveIndices[MAX_PLY];
Move bestMoves[MAX_PLY][MAX_PLY];

Move think(Board *pBoard) {
    // a really stupid search for the moment
    // should manage its time and use an iterative deepening method
    int color = pBoard->info.toPlay == W ? 1 : -1;
    float score = negaMax(0, search_depth,
                          (float) -INFTY,
                          (float) INFTY, color);
    //float score = alphaBeta(0, search_depth, -INFTY, INFTY,
    //                        pBoard->info.toPlay == W);
    printf("%f\n", score);

    return singleBestMove;
}

float quiescentNegaMax(float alpha, float beta, int color) {
    float base_value = evaluate(pBoard);

    if (base_value >= beta) return base_value;
    if (alpha < base_value) alpha = base_value;

    MoveSet moves;
    float value;

    resetMoveSet(&moves);
    generateCapture(pBoard, &moves);

    for (int i = 0; i < moves.totalMoves; i++) {
        if(!checks(pBoard, otherColor(pBoard->info.toPlay))) {
            makeMove(pBoard, moves.moveList[i]);
            value = -quiescentNegaMax(-beta, -alpha, -1*color);
            unmakeMove(pBoard, moves.moveList[i]);

            if (value >= beta) return beta;
            if (value > alpha) alpha = value;
        }
    }
    return alpha;
}

// color is +1 for white and -1 for black, reflecting the consistency that
// for the purposes of board evaluation positive scores favor white
float negaMax(int ply, int depth, float alpha, float beta, int color) {
    if (depth == 0) {
        return quiescentNegaMax(alpha, beta, color);
    }

    float value;
    float bestValue = -INFTY;
    MoveSet moves;

    resetMoveSet(&moves);
    initializeMoveSetQuiet(pBoard, &moves);


    for (int i = 0; i < moves.totalMoves; i++) {
        if (!checks(pBoard, otherColor(pBoard->info.toPlay))) {
            makeMove(pBoard, moves.moveList[i]);
            value = -negaMax(ply+1, depth-1, -beta, -alpha, -1 * color);
            unmakeMove(pBoard, moves.moveList[i]);

            if (value > bestValue) bestValue = value;
            if (value > alpha) {
                if (ply == 0) {
                    printf("%3.1f %3.1f\n", value, alpha);
                    singleBestMove = moves.moveList[i];
                }
                alpha = value;
            }
            if (alpha >= beta)
                break;
        }
    }
    return bestValue;
}

float alphaBeta(int ply, int depth, float alpha, float beta, bool white) {
    // we'll use a negascout variation, without quiescence search,
    // pvs, futility pruning, razoring
    // or any of the extras really, at least for now
    // will be one hell of an unstable search. :P

    // oh, and we'll need a transposition table and move sort too

    bestMoveIndices[ply] = ply;
    // should do quiescence search!
    if(depth == 0) {
        return quiescentNegaMax(alpha, beta, white);
    }


    float value;

    MoveSet moves;
    resetMoveSet(&moves);
    initializeMoveSetQuiet(pBoard, &moves);
    qsort(&moves.moveList, moves.totalMoves, sizeof(Move), compMove);

    if (white) {
        for(int i = 0; i < moves.totalMoves; i++) {
            if (!checks(pBoard, otherColor(pBoard->info.toPlay))) {
                makeMove(pBoard, moves.moveList[i]);
                value = alphaBeta(ply+1, depth-1, alpha, beta, false);
                unmakeMove(pBoard, moves.moveList[i]);

                if(beta <= alpha) {
                    break;
                }


                if(value > alpha) {
                    alpha = value;
                    bestMoves[ply][ply] = moves.moveList[i];

                    for(int j = ply+1; j < bestMoveIndices[ply+1]; j++)
                        bestMoves[ply][j] = bestMoves[ply+1][j];

                    bestMoveIndices[ply] = bestMoveIndices[ply+1];
                }
            }
        }
        return alpha;
    } else {
        for(int i = 0; i < moves.totalMoves; i++) {
            if (!checks(pBoard, otherColor(pBoard->info.toPlay))) {
                makeMove(pBoard, moves.moveList[i]);
                value = alphaBeta(ply+1, depth-1, alpha, beta, false);
                unmakeMove(pBoard, moves.moveList[i]);

                if(beta <= alpha) {
                    break;
                }

                if(value < beta) {
                    beta = value;
                    bestMoves[ply][ply] = moves.moveList[i];

                    for(int j = ply+1; j < bestMoveIndices[ply+1]; j++)
                        bestMoves[ply][j] = bestMoves[ply+1][j];

                    bestMoveIndices[ply] = bestMoveIndices[ply+1];
                }
            }
        }
        return beta;
    }
}
