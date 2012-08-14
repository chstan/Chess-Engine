#ifndef NOTATION_H
#define NOTATION_H

#include "move.h"



Move notationToMove(Board *pBoard, char *notation);

char *moveToNotation(Board *pBoard, Move m);

#endif
