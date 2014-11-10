#ifndef FEN_H
#define FEN_H

#include "board/board.h"

/** Struct: FEN
 *  -----------
 * Wrapper struct for the things required to contain a FEN record.
 * Although we could make the memory footprint smaller, we'd have to dynamically
 * allocate the strings anyway, so we don't save memory, and this way is faster and
 * more convenient.
**/
typedef struct {
    char boardString[73];
    char turnChar;
    char castleString[5];
    char enPassantString[3];
    int halfmoveClock;
    int fullmoveClock;
} FEN;

/** Function: loadFromFEN
 *  ---------------------
 * loadFromFen allows restoration of the board state from a Forsyth-Edwards Notation record.
**/
void loadFromFEN(Board *pBoard, FEN *record);

/** Function: FENFromLine
 *  ---------------------
 * parses a string into FEN
**/
FEN *FENFromLine(const char *fen_string);

/** Function: getFENFromFile
 *  ------------------------
 * getFENFromFile takes a string containing a filename and returns it as a char *.
**/
FEN *getFENFromFile(char *fileName);

#endif
