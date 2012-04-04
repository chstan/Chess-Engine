#ifndef BOARD_H
#define BOARD_H

#include "defines.h"

typedef struct {
	BITBOARD king, queen, rook, bishop, knight, pawn;
	BITBOARD occupied;
} PlayerPieces;

typedef struct {
	PlayerPieces white;
	PlayerPieces black;
	
	int Material;
	
	int square[64];
	int kings[2];
	
	signed char pieces[2][7];
	signed char totalPieces;
} Position;

typedef struct {
	unsigned char nextMove;
	unsigned char castleWhite;
	unsigned char castleBlack;
	
	int enPassantSquare;
	int staleMoves;
	
	bool displayRotated;
} BoardInfo;

#endif