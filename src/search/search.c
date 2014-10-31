#include <stdio.h>

#include "search.h"
#include "../move/movegen.h"
#include "../eval/eval.h"
#include "../board/board.h"
#include "../defines.h"
#include "../extglobals.h"

// kind of a hack, would be better if we had
// proper data structures, vectors or stacks would be nice
// C++!
int bestMoveIndices[MAX_PLY];
Move bestMoves[MAX_PLY][MAX_PLY];

Move think(Board *pBoard) {
    // a really stupid search for the moment
    // should manage its time and use an iterative deepening method
    int searchDepth = 4;

    float score = alphaBeta(0, searchDepth, -INFTY, INFTY,
                            pBoard->info.toPlay == W);
    printf("%f\n", score);

    return bestMoves[0][0];
}

float alphaBeta(int ply, int depth, float alpha, float beta, bool white) {
    // we'll use a negascout variation, without quiescence search, pvs, futility pruning, razoring
    // or any of the extras really, at least for now
    // will be one hell of an unstable search. :P

    // oh, and we'll need a transposition table and move sort too

    bestMoveIndices[ply] = ply;
    // should do quiescence search!
    if(depth == 0) {
        return evaluate(pBoard);
    }


    int value;

    MoveSet moves;
    resetMoveSet(&moves);
    initializeMoveSetQuiet(pBoard, &moves);

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
