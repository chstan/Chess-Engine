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
    char board_string[73];
    char turn_char;
    char castle_string[5];
    char en_passant_string[3];
    int halfmove_clock;
    int fullmove_clock;
} FEN;

/** Function: load_from_FEN
 *  ---------------------
 * load_from_FEN allows restoration of the board state from a Forsyth-Edwards Notation record.
**/
void load_from_FEN(Board *p_board, FEN *record);

/** Function: FEN_from_line
 *  ---------------------
 * parses a string into FEN
**/
FEN *FEN_from_line(const char *fen_string);

/** Function: get_FEN_from_file
 *  ------------------------
 * get_FEN_from_file takes a string containing a filename and returns it as a char *.
**/
FEN *get_FEN_from_file(char *fileName);

#endif
