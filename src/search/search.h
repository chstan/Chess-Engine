#ifndef SEARCH_H
#define SEARCH_H

#include "../move/move.h"
#include "../board/board.h"

#define MAX_PLY 20

Move think(Board *pBoard);

float quiescentNegaMax(float alpha, float beta, int color);

float negaMax(int ply, int depth, float alpha, float beta, int color);


#endif
