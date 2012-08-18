#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "fen.h"
#include "move/notation.h"
#include "defines.h"
#include "extglobals.h"

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

static void castleFromFen(char *castleFen, int *castleWhite, int *castleBlack) {
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

void loadFromFEN(Board *pBoard, char *fen) {
	char boardString[73];
	char turnChar;
	char castleString[5];
	char enPassantString[3];
	int halfmoveClock; // since pawn advance or capture
	int fullmoveClock;
	if(sscanf(fen, "%72s %c %4s %2s %d %d", boardString, &turnChar, castleString, enPassantString, &halfmoveClock, &fullmoveClock) != 6) {
		printf("sscanf failed to read from:\n%s\n", fen);
		abort();
	}
	
	// we first clean everything up, this makes the rest of the setup easier
	resetBoard(pBoard);
	
	int rank = 7;
	int idx = A8; // for some reason, FEN records start addressing with rank 8
	for(char *iter = boardString; *iter != '\0'; iter++) {
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
	
	int toPlay = (turnChar == 'w') ? WHITE : BLACK;
	int castleWhite = 0, castleBlack = 0;
	castleFromFen(castleString, &castleWhite, &castleBlack);
	int epSquare = getEnPassantSquare(enPassantString);
	
	void initBoardFromSquares(Board* pBoard, unsigned char toPlay, int staleMoves, int castleW, 
		int castleB, int enPassantSquare, int turnCount);
	initBoardFromSquares(pBoard, toPlay, halfmoveClock, castleWhite, castleBlack, epSquare, fullmoveClock);
	
}

