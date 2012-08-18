#ifndef EVALHELPERS_H
#define EVALHELPERS_H

#include "../board/board.h"

int doubledPawns(Board *pBoard);

int tripledPawns(Board *pBoard);

float bishopMobility(Board *pBoard);

float rookMobility(Board *pBoard);

float kingSafety(Board *pBoard);

int control(Board *pBoard, int square);

float controlOfCenter(Board *pBoard);

#endif
