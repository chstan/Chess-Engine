#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "fen.h"
#include "move/notation.h"
#include "defines.h"
#include "extglobals.h"

FEN *FEN_from_line(const char *line) {
    char board_string_temp[73];
    char turn_char_temp;
    char castle_string_temp[5];
    char en_passant_string_temp[3];
    int halfmove_clock_temp;
    int fullmove_clock_temp;
    if(sscanf(line, "%72s %c %4s %2s %d %d", board_string_temp, &turn_char_temp, castle_string_temp,
                        en_passant_string_temp, &halfmove_clock_temp, &fullmove_clock_temp) == 6) {
        FEN *record = malloc(sizeof(FEN));
        strncpy(record->board_string, board_string_temp, 73);
        strncpy(record->castle_string, castle_string_temp, 5);
        strncpy(record->en_passant_string, en_passant_string_temp, 3);
        record->turn_char = turn_char_temp;
        record->halfmove_clock = halfmove_clock_temp;
        record->fullmove_clock = fullmove_clock_temp;
        return record;
    }
    return NULL;
}

FEN *get_FEN_from_file(char *fileName) {
    // should return a valid FEN string
    FILE *fp = fopen(fileName, "r");
    if(!fp) {
        fprintf(stderr, "get_FEN_from_file: unable to load from %s\n", fileName);
    }

    char lineBuffer[160];
    FEN *record;

    while(fgets(lineBuffer, sizeof(lineBuffer), fp)) {
        if((record = FEN_from_line(lineBuffer))) {
            fclose(fp);
            return record;
        }
    }

    fclose(fp);
    return NULL;
}

static int getPieceFromChar(char pieceChar) {
    switch(pieceChar) {
        case 'P':
        return WHITE_PAWN;
        break;
        case 'N':
        return WHITE_KNIGHT;
        break;
        case 'B':
        return WHITE_BISHOP;
        break;
        case 'R':
        return WHITE_ROOK;
        break;
        case 'Q':
        return WHITE_QUEEN;
        break;
        case 'K':
        return WHITE_KING;
        break;
        case 'p':
        return BLACK_PAWN;
        break;
        case 'n':
        return BLACK_KNIGHT;
        break;
        case 'b':
        return BLACK_BISHOP;
        break;
        case 'r':
        return BLACK_ROOK;
        break;
        case 'q':
        return BLACK_QUEEN;
        break;
        case 'k':
        return BLACK_KING;
        break;
    }
    return NO_PIECE;
}

static void castleFromFEN(char *castleFen, int *castle_white, int *castle_black) {
    if(strchr(castleFen, 'Q'))
        *castle_white |= CAN_CASTLE_OOO;
    if(strchr(castleFen, 'K'))
        *castle_white |= CAN_CASTLE_OO;
    if(strchr(castleFen, 'q'))
        *castle_black |= CAN_CASTLE_OOO;
    if(strchr(castleFen, 'k'))
        *castle_black |= CAN_CASTLE_OO;
}

static int getEnPassantSquare(char *epString) {
    if(strlen(epString) != 2) return INVALID_SQUARE;
    return get_square(epString);
}

void load_from_FEN(Board *p_board, FEN *record) {
    // we first clean everything up, this makes the rest of the setup easier
    reset_board(p_board);

    if(!record) return;

    int rank = 7;
    int idx = A8; // for some reason, FEN records start addressing with rank 8
    for(char *iter = record->board_string; *iter != '\0'; iter++) {
        if(*iter == '/') {
            rank--;
            idx = rank * 8;
        }
        if(isdigit(*iter)) {
            for(int skip = *iter - '1' + 1; skip > 0; skip--) {
                p_board->position.square[idx] = EMPTY;
                idx++;
            }
        }
        else if(isalpha(*iter)) {
            p_board->position.square[idx] = getPieceFromChar(*iter);
            idx++;
        }
    }

    int to_play = (record->turn_char == 'w') ? WHITE : BLACK;
    int castle_white = 0, castle_black = 0;
    castleFromFEN(record->castle_string, &castle_white, &castle_black);
    int ep_square = getEnPassantSquare(record->en_passant_string);

    init_board_from_squares(p_board, to_play, record->halfmove_clock, castle_white, castle_black, ep_square, record->fullmove_clock);

}
