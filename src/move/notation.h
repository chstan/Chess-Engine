#ifndef NOTATION_H
#define NOTATION_H

#include "move.h"

/** Function: get_square
 *  -------------------
 * get_square converts a string designating a board position into the appropriate
 * numeric. Since this is used in a few other places, we'll keep the protoype here.
**/
int get_square(char *sq);

/** Function: notation_to_move
 *  ------------------------
 * Takes a string representing a move in algebraic chess notation and converts
 * it into the internal move format (see move.h) using the current board state
 * to resolve ambiguities where they are encountered.
**/
Move notation_to_move(Board *p_board, char *notation);

/** Function: move_to_notation
 *  ------------------------
 * The reverse of notation_to_move, takes a move in the internal format and converts
 * it into algebraic chess notation. It does not however add '+'/'#'/'++' at the end
 * of a move for checks or mates. The resulting string is malloced, and so must be freed
 * later by the client.
**/
char *move_to_notation(Board *p_board, Move m);

/** Function coord_notation_to_move
 *  -------------------------------
 * The same as notation_to_move, but instead of using algebraic notation we use
 * coordinate location, which is always specified as startsquare - endsquare
 **/
Move coord_notation_to_move(Board *p_board, char *notation);

/** Function move_to_coord_notation
 *  -------------------------------
 * The same as move_to_notation, but instead of using algebraic notation we use
 * coordinate location, which is always specified as startsquare - endsquare
 **/
char *move_to_coord_notation(Move m);


#endif
