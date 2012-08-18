#ifndef NOTATION_H
#define NOTATION_H

#include "move.h"

/** Function: getSquare
 *  -------------------
 * getSquare converts a string designating a board position into the appropriate
 * numeric. Since this is used in a few other places, we'll keep the protoype here.
**/
int getSquare(char *sq);

/** Function: notationToMove
 *  ------------------------
 * Takes a string representing a move in algebraic chess notation and converts
 * it into the internal move format (see move.h) using the current board state
 * to resolve ambiguities where they are encountered.
**/
Move notationToMove(Board *pBoard, char *notation);

/** Function: moveToNotation
 *  ------------------------
 * The reverse of notationToMove, takes a move in the internal format and converts
 * it into algebraic chess notation. It does not however add '+'/'#'/'++' at the end
 * of a move for checks or mates. The resulting string is malloced, and so must be freed
 * later by the client.
**/
char *moveToNotation(Board *pBoard, Move m);

#endif
