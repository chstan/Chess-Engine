#ifndef FEN_H
#define FEN_H

#include "board/board.h"

/** Function: loadFromFEN
 *  ---------------------
 * loadFromFen allows restoration of the board state from a Forsyth-Edwards Notation record.
**/
void loadFromFEN(Board *pBoard, char *fen);

/** Function: getFENFromFile
 *  ------------------------
 * getFENFromFile takes a string containing a filename and returns it as a char *.
**/
char *getFENFromFile(char *fileName);

#endif
