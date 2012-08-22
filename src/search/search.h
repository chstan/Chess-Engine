#ifndef SEARCH_H
#define SEARCH_H

#include "../move/move.h"

#define INFTY 999999

#define MAX_PLY 20

Move think();

int alphaBeta(int ply, int depth, int alpha, int beta);

#endif
