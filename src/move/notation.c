#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "notation.h"
#include "move.h"
#include "../defines.h"
#include "../extglobals.h"


Move queenSide(unsigned char color) {
	if(color == W) {
		return (moveF(0, 0, 0, 1, 0, 0, WHITE_KING, E1, C1));
	} else {
		return (moveF(0, 0, 1, 0, 0, 0, BLACK_KING, E8, C8));
	}
	return 0;
}

Move kingSide(unsigned char color) {
	if(color == W) {
		return moveF(0, 0, 0, 1, 0, 0, WHITE_KING, E1, G1);
	} else {
		return moveF(0, 0, 1, 0, 0, 0, BLACK_KING, E8, G8);
	}
	return 0;
}

int getSquare(char *sq) {
	for(int i = 0; i < 64; i++) {
		if(!strcmp(sq, SQUARENAME[i]))
			return i;
	}
	return INVALID_SQUARE;
}

Move notationToMove(Board *pBoard, int totalArgs, char **tokens) {
	unsigned char color = pBoard->info.toPlay;
	if(!strcmp(tokens[1], "O-O-O")) {
		return queenSide(color);
	} else if (!strcmp(tokens[1], "O-O")) {
		return kingSide(color);
	}
	unsigned int piece, orig, dest;
	Move m = 0;
	if(!strcmp(tokens[1], "p")) {
		if(totalArgs >= 3) {
			piece = (color ? BLACK_PAWN : WHITE_PAWN);
			orig = getSquare(tokens[2]);
			dest = getSquare(tokens[3]);
			// capt, prom, en passant	
		} else {
			printf("Improper number of arguments to specify a pawn move.\n\n");
			return 0;
		}
		
	} else {
		if(totalArgs >= 3) {
			if(!strcmp(tokens[1], "b")) piece = (color ? BLACK_BISHOP : WHITE_BISHOP);
			if(!strcmp(tokens[1], "k")) piece = (color ? BLACK_KING : WHITE_KING);
			if(!strcmp(tokens[1], "q")) piece = (color ? BLACK_QUEEN : WHITE_QUEEN);
			if(!strcmp(tokens[1], "n")) piece = (color ? BLACK_KNIGHT : WHITE_KNIGHT);
			if(!strcmp(tokens[1], "r")) piece = (color ? BLACK_ROOK : WHITE_ROOK);
			orig = getSquare(tokens[2]);
			dest = getSquare(tokens[3]);			
		} else {
			printf("Improper number of arguments to specify a non-pawn move.\n\n");
			return 0;
		}
	}
	m = move(0, piece, orig, dest);
	return m;
}
