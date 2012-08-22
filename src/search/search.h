#ifndef SEARCH_H
#define SEARCH_H

#include "../move/move.h"
#include "../board/board.h"

#define INFTY 999999

#define MAX_PLY 20

Move think(Board *pBoard);

int alphaBeta(int ply, int depth, int alpha, int beta);

#endif
