#include "board.h"
#include "protos.h"
#include "defines.h"
#include "globals.h"

void resetBoard(Board *pBoard) {
	int i;
	for(i = 0; i < 64; i++) board->position.square[i] = EMPTY;
	pBoard->position.square[A1] = WHITE_ROOK;
	pBoard->position.square[B1] = WHITE_KNIGHT;
	pBoard->position.square[C1] = WHITE_BISHOP;
	pBoard->position.square[D1] = WHITE_QUEEN;
	pBoard->position.square[E1] = WHITE_KING;
	pBoard->position.square[F1] = WHITE_BISHOP;
	pBoard->position.square[G1] = WHITE_KNIGHT;
	pBoard->position.square[H1] = WHITE_ROOK;
	pBoard->position.square[A2] = WHITE_PAWN;
	pBoard->position.square[B2] = WHITE_PAWN;
	pBoard->position.square[C2] = WHITE_PAWN;
	pBoard->position.square[D2] = WHITE_PAWN;
	pBoard->position.square[E2] = WHITE_PAWN;
	pBoard->position.square[F2] = WHITE_PAWN;
	pBoard->position.square[G2] = WHITE_PAWN;
	pBoard->position.square[H2] = WHITE_PAWN;
	
	pBoard->position.square[A8] = BLACK_ROOK;
	pBoard->position.square[B8] = BLACK_KNIGHT;
	pBoard->position.square[C8] = BLACK_BISHOP;
	pBoard->position.square[D8] = BLACK_QUEEN;
	pBoard->position.square[E8] = BLACK_KING;
	pBoard->position.square[F8] = BLACK_BISHOP;
	pBoard->position.square[G8] = BLACK_KNIGHT;
	pBoard->position.square[H8] = BLACK_ROOK;
	pBoard->position.square[A7] = BLACK_PAWN;
	pBoard->position.square[B7] = BLACK_PAWN;
	pBoard->position.square[C7] = BLACK_PAWN;
	pBoard->position.square[D7] = BLACK_PAWN;
	pBoard->position.square[E7] = BLACK_PAWN;
	pBoard->position.square[F7] = BLACK_PAWN;
	pBoard->position.square[G7] = BLACK_PAWN;
	pBoard->position.square[H7] = BLACK_PAWN;
	
	board->info.displayRotated = false;
	
	initBoardFromSquares(pBoard, WHITE, 0, CAN_CASTLE_OO + CAN_CASTLE_OOO,
		CAN_CASTLE_OO + CAN_CASTLE_OOO, 0);
	return void;
}

void initBoardFromSquares(Board* pBoard, unsigned char next,
	int stale, int castleW, int castleB, int enPSq) {
	int i;
	
	pBoard->position.white.king = 0;
	pBoard->position.white.queen = 0;
	pBoard->position.white.rook = 0;
	pBoard->position.white.bishop = 0;
	pBoard->position.white.knight = 0;
	pBoard->position.white.pawn = 0;
	pBoard->position.black.pieces = 0;
	
	pBoard->position.black.king = 0;
	pBoard->position.black.queen = 0;
	pBoard->position.black.rook = 0;
	pBoard->position.black.bishop = 0;
	pBoard->position.black.knight = 0;
	pBoard->position.black.pawn = 0;
	pBoard->position.black.pieces = 0;
	
	pBoard->position.occupied = 0;
	
	for(i = 0; i < 64; i++) {
		if(pBoard->position.square[i] == WHITE_PAWN) pBoard->position.white.pawn = pBoard->position.white.pawn | BITSET[i];
		if(pBoard->position.square[i] == WHITE_KING) pBoard->position.white.king = pBoard->position.white.king | BITSET[i];
		if(pBoard->position.square[i] == WHITE_QUEEN) pBoard->position.white.queen = pBoard->position.white.queen | BITSET[i];
		if(pBoard->position.square[i] == WHITE_BISHOP) pBoard->position.white.bishop = pBoard->position.white.bishop | BITSET[i];
		if(pBoard->position.square[i] == WHITE_KNIGHT) pBoard->position.white.knight = pBoard->position.white.knight | BITSET[i];
		if(pBoard->position.square[i] == WHITE_ROOK) pBoard->position.white.rook = pBoard->position.white.rook | BITSET[i];
		
		if(pBoard->position.square[i] == BLACK_PAWN) pBoard->position.black.pawn = pBoard->position.black.pawn | BITSET[i];
		if(pBoard->position.square[i] == BLACK_KING) pBoard->position.black.king = pBoard->position.black.king | BITSET[i];
		if(pBoard->position.square[i] == BLACK_QUEEN) pBoard->position.black.queen = pBoard->position.black.queen | BITSET[i];
		if(pBoard->position.square[i] == BLACK_BISHOP) pBoard->position.black.bishop = pBoard->position.black.bishop | BITSET[i];
		if(pBoard->position.square[i] == BLACK_KNIGHT) pBoard->position.black.knight = pBoard->position.black.knight | BITSET[i];
		if(pBoard->position.square[i] == BLACK_ROOK) pBoard->position.black.rook = pBoard->position.black.rook | BITSET[i];
	}
	
	
}