#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "fen.h"
#include "move/notation.h"
#include "defines.h"
#include "extglobals.h"

static FEN *FENFromLine(char *line) {
    char boardStringTemp[73];
    char turnCharTemp;
    char castleStringTemp[5];
    char enPassantStringTemp[3];
    int halfmoveClockTemp;
    int fullmoveClockTemp;
    if(sscanf(line, "%72s %c %4s %2s %d %d", boardStringTemp, &turnCharTemp, castleStringTemp,
                        enPassantStringTemp, &halfmoveClockTemp, &fullmoveClockTemp) == 6) {
        FEN *record = malloc(sizeof(FEN));
        strncpy(record->boardString, boardStringTemp, 73);
        strncpy(record->castleString, castleStringTemp, 5);
        strncpy(record->enPassantString, enPassantStringTemp, 3);
        record->turnChar = turnCharTemp;
        record->halfmoveClock = halfmoveClockTemp;
        record->fullmoveClock = fullmoveClockTemp;
        return record;
    }
    return NULL;
}

FEN *getFENFromFile(char *fileName) {
    // should return a valid FEN string
    FILE *fp = fopen(fileName, "r");
    if(!fp) {
        fprintf(stderr, "getFENFromFile: unable to load from %s\n", fileName);
    }

    char lineBuffer[160];
    FEN *record;

    while(fgets(lineBuffer, sizeof(lineBuffer), fp)) {
        if((record = FENFromLine(lineBuffer))) {
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

static void castleFromFEN(char *castleFen, int *castleWhite, int *castleBlack) {
    if(strchr(castleFen, 'Q'))
        *castleWhite |= CAN_CASTLE_OOO;
    if(strchr(castleFen, 'K'))
        *castleWhite |= CAN_CASTLE_OO;
    if(strchr(castleFen, 'q'))
        *castleBlack |= CAN_CASTLE_OOO;
    if(strchr(castleFen, 'k'))
        *castleBlack |= CAN_CASTLE_OO;
}

static int getEnPassantSquare(char *epString) {
    if(strlen(epString) != 2) return INVALID_SQUARE;
    return getSquare(epString);
}

void loadFromFEN(Board *pBoard, FEN *record) {
    // we first clean everything up, this makes the rest of the setup easier
    resetBoard(pBoard);

    if(!record) return;

    int rank = 7;
    int idx = A8; // for some reason, FEN records start addressing with rank 8
    for(char *iter = record->boardString; *iter != '\0'; iter++) {
        if(*iter == '/') {
            rank--;
            idx = rank * 8;
        }
        if(isdigit(*iter)) {
            for(int skip = *iter - '1' + 1; skip > 0; skip--) {
                pBoard->position.square[idx] = EMPTY;
                idx++;
            }
        }
        else if(isalpha(*iter)) {
            pBoard->position.square[idx] = getPieceFromChar(*iter);
            idx++;
        }
    }

    int toPlay = (record->turnChar == 'w') ? WHITE : BLACK;
    int castleWhite = 0, castleBlack = 0;
    castleFromFEN(record->castleString, &castleWhite, &castleBlack);
    int epSquare = getEnPassantSquare(record->enPassantString);

    initBoardFromSquares(pBoard, toPlay, record->halfmoveClock, castleWhite, castleBlack, epSquare, record->fullmoveClock);

}
