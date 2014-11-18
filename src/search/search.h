#ifndef SEARCH_H
#define SEARCH_H

#include "../move/move.h"
#include "../board/board.h"

#define MAX_PLY 20

void *threadable_think(void *arg);
Move think(Board *pBoard);

int quiescentNegaMax(int ply, int alpha, int beta, int color, Move *pm);

int negaMax(int ply, int depth, int alpha, int beta, int color, Move *pm,
            bool check_for_stop);

#endif
