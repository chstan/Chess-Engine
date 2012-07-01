#ifndef NOTATION_H
#define NOTATION_H

#include "move.h"

Move queenSide(unsigned char color);

Move kingSide(unsigned char color);

int getSquare(char *sq);

Move notationToMove(Board *pBoard);

#endif
