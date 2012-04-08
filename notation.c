#include "move.h"
#include "defines.h"
#include "extglobals.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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

Move notationToMove(Board *pBoard) {
	unsigned char color = pBoard->info.nextMove;
	if(!strcmp(TOKENS[1], "O-O-O")) {
		return queenSide(color);
	} else if (!strcmp(TOKENS[1], "O-O")) {
		return kingSide(color);
	}
	unsigned int piece, orig, dest;
	Move m = 0;
	int totalArgs = TOKEN_COUNT-1;
	if(!strcmp(TOKENS[1], "p")) {
		if(totalArgs >= 3) {
			piece = (color ? BLACK_PAWN : WHITE_PAWN);
			orig = atoi(TOKENS[2]);
			dest = atoi(TOKENS[3]);
			// capt, prom, en passant	
		} else {
			printf("Improper number of arguments to specify a pawn move.\n\n");
			return 0;
		}
		
	} else {
		if(totalArgs >= 3) {
			if(!strcmp(TOKENS[1], "b")) piece = (color ? BLACK_BISHOP : WHITE_BISHOP);
			if(!strcmp(TOKENS[1], "k")) piece = (color ? BLACK_KING : WHITE_KING);
			if(!strcmp(TOKENS[1], "q")) piece = (color ? BLACK_QUEEN : WHITE_QUEEN);
			if(!strcmp(TOKENS[1], "n")) piece = (color ? BLACK_KNIGHT : WHITE_KNIGHT);
			if(!strcmp(TOKENS[1], "r")) piece = (color ? BLACK_ROOK : WHITE_ROOK);
			orig = atoi(TOKENS[2]);
			dest = atoi(TOKENS[3]);			
		} else {
			printf("Improper number of arguments to specify a non-pawn move.\n\n");
			return 0;
		}
	}
	m = move(0, piece, orig, dest);
	return m;
}