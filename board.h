#ifndef BOARD_H
#define BOARD_H

#include "defines.h"

#define sq(RANK, FILE) 8*(RANK-1)+(FILE-1)

typedef struct {
	BitBoard king, queen, rook, bishop, knight, pawn;
	BitBoard pieces;
} PlayerPieces;

typedef struct {
	PlayerPieces white;
	PlayerPieces black;
	BitBoard occupied;
	
	int square[64];
	int kings[2];
	
	signed char pieces[2][7];
	signed char totalPieces;
} Position;

typedef struct {
	int material;
	int whiteMaterial;
	int blackMaterial;
	
	unsigned char nextMove;
	unsigned char castleWhite;
	unsigned char castleBlack;
	
	int enPassantSquare;
	int staleMoves;
	
	bool displayRotated;
} BoardInfo;

typedef struct {
	Position position;
	BoardInfo info;
} Board;

#endif