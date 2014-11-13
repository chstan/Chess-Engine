#ifndef EVALHELPERS_H
#define EVALHELPERS_H

#include "../board/board.h"

void initEval();

int all_piece_square_scores (Board *pBoard);

int doubledPawns(Board *pBoard);

int tripledPawns(Board *pBoard);

int passedPawns(Board *pBoard);

int outsidePassedPawns(Board *pBoard);

int bishopMobility(Board *pBoard);

int rookMobility(Board *pBoard);

int kingSafety(Board *pBoard);

int control(Board *pBoard, int square);

int controlOfCenter(Board *pBoard);

#endif
