#ifndef BOARD_H
#define BOARD_H

#include "defines.h"

tyepdef struct {
	Bitboard whiteKing, whiteQueens, whiteBishops, whiteKnights, whitePawns;
	Bitboard blackKing, blackQueens, blackBishops, blackKnights, blackPawns;
	Bitboard whitePieces, blackPieces, occupiedSquares;
	
	unsigned char nextMove;
	unsigned char castleWhite;
	unsigned char castleBlack;
	int enPassantSquare;
	int staleMoves;
	
	int Material;
	
	int square[64];
	
	bool viewRotated;
	
} Board;

void initBoard(Board &b);
void initBoardFromSquares(Board &b, int input[64], unsigned char next,
	int staleMoves, int castleW, int castleB, int enPassantSquare);
void initBoardFromFen(Board &b, char fen[], char fenColor[], char fenCastling[],
	char fenEnPassant[], int fenHalfMoveClock, int fenFullMoveNumber);
void displayBoard(Board &b);

#endif