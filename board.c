#include "board.h"
#include "protos.h"
#include "defines.h"
#include "extglobals.h"
#include "assert.h"
#include "stdio.h"

void resetBoard(Board *pBoard) {
	int i;
	for(i = 0; i < 64; i++) pBoard->position.square[i] = EMPTY;
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
	
	pBoard->info.displayRotated = false;
	
	initBoardFromSquares(pBoard, WHITE, 0, CAN_CASTLE_OO + CAN_CASTLE_OOO,
		CAN_CASTLE_OO + CAN_CASTLE_OOO, 0);
	return;
}

void initBoardFromSquares(Board* pBoard, unsigned char nextMove,
	int staleMoves, int castleW, int castleB, int enPassantSquare) {
	
	resetBB(pBoard);
	updateBBFromSquares(pBoard);
	updateAggregateBB(pBoard);
	updateMaterialFromBB(pBoard);
	updateKingsFromBB(pBoard);
	updatePieceCountsFromBB(pBoard);
	
	pBoard->info.nextMove = nextMove;
	pBoard->info.castleWhite = castleW;
	pBoard->info.castleBlack = castleB;
	pBoard->info.enPassantSquare = enPassantSquare;
	pBoard->info.staleMoves = staleMoves;
	return;
}

void displayBoard(Board *pBoard) {
	const char *divider = "+-----+-----+-----+-----+-----+-----+-----+-----+";
	
	int i, j;
	for(i = 0; i < 8; i++) {
		printf("%s\n", divider);
		for(j = 0; j < 8; j++) {
			switch(pBoard->info.displayRotated) {
				case false:
					printf("| %s ", PIECENAMES[pBoard->position.square[sq(8-i, j+1)]]);	
				break;
				case true:
					printf("| %s ", PIECENAMES[pBoard->position.square[sq(i+1, j+1)]]);
				break;
			}
		}
		printf("|\n");
	}
	printf("%s\n", divider);
	return;
}

void rotateBoard(Board *pBoard) {
	if(pBoard->info.displayRotated) {
		pBoard->info.displayRotated = false;
	} else {
		pBoard->info.displayRotated = true;
	}
	return;
}

//---------------------AUX FUNCTIONS-------------------------

void updatePieceCountsFromBB(Board *pBoard) {
	// Slow. Consider looping through squares
	pBoard->position.pieces[WHITE][PAWN] = countBits(pBoard->position.white.pawn);
	pBoard->position.pieces[WHITE][BISHOP] = countBits(pBoard->position.white.bishop);
	pBoard->position.pieces[WHITE][KNIGHT] = countBits(pBoard->position.white.knight);
	pBoard->position.pieces[WHITE][ROOK] = countBits(pBoard->position.white.rook);
	pBoard->position.pieces[WHITE][QUEEN] = countBits(pBoard->position.white.queen);
	pBoard->position.pieces[WHITE][KING] = countBits(pBoard->position.white.king);
	pBoard->position.pieces[WHITE][TOTAL] = countBits(pBoard->position.white.pieces);
	
	pBoard->position.pieces[BLACK][PAWN] = countBits(pBoard->position.black.pawn);
	pBoard->position.pieces[BLACK][BISHOP] = countBits(pBoard->position.black.bishop);
	pBoard->position.pieces[BLACK][KNIGHT] = countBits(pBoard->position.black.knight);
	pBoard->position.pieces[BLACK][ROOK] = countBits(pBoard->position.black.rook);
	pBoard->position.pieces[BLACK][QUEEN] = countBits(pBoard->position.black.queen);
	pBoard->position.pieces[BLACK][KING] = countBits(pBoard->position.black.king);
	pBoard->position.pieces[BLACK][TOTAL] = countBits(pBoard->position.black.pieces);
	
	pBoard->position.totalPieces = countBits(pBoard->position.occupied);
	return;
}

void updateKingsFromBB(Board *pBoard) {
	assert(countBits(pBoard->position.white.king) == 1
			&& countBits(pBoard->position.black.king) == 1);
	pBoard->position.kings[WHITE] = LSB(pBoard->position.white.king);
	pBoard->position.kings[BLACK] = LSB(pBoard->position.black.king);
	return;
}

void resetBB(Board *pBoard) {
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
	return;
}

void updateAggregateBB(Board *pBoard) {
	pBoard->position.white.pieces = pBoard->position.white.pawn |
																	pBoard->position.white.king |
																	pBoard->position.white.queen |
																	pBoard->position.white.bishop |
																	pBoard->position.white.knight |
																	pBoard->position.white.rook;

	pBoard->position.black.pieces = pBoard->position.black.pawn |
																	pBoard->position.black.king |
																	pBoard->position.black.queen |
																	pBoard->position.black.bishop |
																	pBoard->position.black.knight |
																	pBoard->position.black.rook;
																		
	pBoard->position.occupied = pBoard->position.white.pieces |
															pBoard->position.black.pieces;
	return;
}

void updateBBFromSquares(Board *pBoard) {
	int i;
	for(i = 0; i < 64; i++) {
		if(pBoard->position.square[i] == WHITE_PAWN) pBoard->position.white.pawn |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_KING) pBoard->position.white.king |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_QUEEN) pBoard->position.white.queen |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_BISHOP) pBoard->position.white.bishop |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_KNIGHT) pBoard->position.white.knight |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_ROOK) pBoard->position.white.rook |= BITSET[i];
		
		if(pBoard->position.square[i] == BLACK_PAWN) pBoard->position.black.pawn |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_KING) pBoard->position.black.king |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_QUEEN) pBoard->position.black.queen |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_BISHOP) pBoard->position.black.bishop |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_KNIGHT) pBoard->position.black.knight |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_ROOK) pBoard->position.black.rook |= BITSET[i];
	}
	return;
}

void updateMaterialFromBB(Board *pBoard) {
	pBoard->info.material = countBits(pBoard->position.white.pawn) * PAWN_VALUE
												+	countBits(pBoard->position.white.bishop) * BISHOP_VALUE
												+	countBits(pBoard->position.white.knight) * KNIGHT_VALUE
												+	countBits(pBoard->position.white.rook) * ROOK_VALUE
												+	countBits(pBoard->position.white.queen) * QUEEN_VALUE
													
												-	countBits(pBoard->position.black.pawn) * PAWN_VALUE
												-	countBits(pBoard->position.black.bishop) * BISHOP_VALUE
												-	countBits(pBoard->position.black.knight) * KNIGHT_VALUE
												-	countBits(pBoard->position.black.rook) * ROOK_VALUE
												-	countBits(pBoard->position.black.queen) * QUEEN_VALUE;
	
	return;
}