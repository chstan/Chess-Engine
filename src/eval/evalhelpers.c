#include <stdio.h>
#include <stdlib.h>

#include "evalhelpers.h"
#include "../defines.h"
#include "../extglobals.h"
#include "../bit.h"

// TODO
/*
 * Rook, Bishop, Queen, Knight outposts and outpost value tables
 * Material hash tables
 * Pawn value hash tables
 * Endgame hash tables
 * Tempo Bonus, Lost tempo penalty
 * Trapped rook no castling penalty
 * Uncatchable pawns
 *
 * Maintain attack tables which should be incrementally updated
 *
 * Normalize scores with tunable constants, this will allow machine learning
 * to tune the evaluators externally
 */


// we could generalize to x files, and should
BitBoard passedPawn[2][64];
BitBoard outsidePassedPawn[2][64];
BitBoard kingSafetyZone[2][64];


void initEval() {
	for(int rank = 0; rank < 8; rank++) {
		for(int file = 0; file < 8; file++) {
			int square = SQ(rank+1, file+1);
			
			BitBoard passedMask = 0;
			BitBoard outsideMask = 0;
			for(int fileIndex = 0; fileIndex < 8; fileIndex++) {
				if(abs(fileIndex - file) < 2)
					passedMask |= fileBB[fileIndex];
				if(abs(fileIndex - file) < 3)
					outsideMask |= fileBB[fileIndex];
			}

			if(square < A8)
				passedPawn[WHITE][square] = passedMask & ~(BITSET(square + 2) - 1);
			else
				passedPawn[WHITE][square] = 0;
			
			if(square >= A2)
				passedPawn[BLACK][square] = passedMask & (BITSET(square - 6) - 1);
			else
				passedPawn[BLACK][square] = 0;
			
			
			if(square < A8)
				outsidePassedPawn[WHITE][square] = outsideMask & ~(BITSET(square + 3) - 1);
			else
				outsidePassedPawn[WHITE][square] = 0;

			if(square >= A2)
				outsidePassedPawn[BLACK][square] = outsideMask & (BITSET(square - 5) - 1);
			else
				outsidePassedPawn[BLACK][square] = 0;
		}
	}
}



int doubledPawns(Board *pBoard) {
	int doubledPawns = 0;
	for(int currentFile = 0; currentFile < 8; currentFile++) {
		if(countBits(pBoard->position.pieceBB[WHITE_PAWN] & fileBB[currentFile]) == 2) doubledPawns++;
		if(countBits(pBoard->position.pieceBB[BLACK_PAWN] & fileBB[currentFile]) == 2) doubledPawns--;
	}
	return doubledPawns;
}

int tripledPawns(Board *pBoard) {
	int tripledPawns = 0;
	for(int currentFile = 0; currentFile < 8; currentFile++) {
		if(countBits(pBoard->position.pieceBB[WHITE_PAWN] & fileBB[currentFile]) == 3) tripledPawns++;
		if(countBits(pBoard->position.pieceBB[BLACK_PAWN] & fileBB[currentFile]) == 3) tripledPawns--;
	}
	return tripledPawns;
}

int passedPawns(Board *pBoard) {
	int passedPawn = 0;
	// not as fast as an optimized bitboard method, but this is all still proof of concept
	for(int square = A1; square <= H8; square++) {
		int piece = pBoard->position.square[square];
		if(piece == WHITE_PAWN) {
			if(!(passedPawn[WHITE][square] & pBoard->position.pieceBB[BLACK_PAWN]))
				passedPawn++;
		} else if(piece == BLACK_PAWN) {
			if(!(passedPawn[BLACK][square] & pBoard->position.pieceBB[WHITE_PAWN]))
				passedPawn--;
		}
	}
	return passedPawn;
}

int outsidePassedPawns(Board *pBoard) {
	int outsidePawn = 0;
	// not as fast as an optimized bitboard method, but this is all still proof of concept
	for(int square = A1; square <= H8; square++) {
		int piece = pBoard->position.square[square];
		if(piece == WHITE_PAWN) {
			if(!(outsidePassedPawn[WHITE][square] & pBoard->position.pieceBB[BLACK_PAWN]))
				outsidePawn++;
		} else if(piece == BLACK_PAWN) {
			if(!(outsidePassedPawn[BLACK][square] & pBoard->position.pieceBB[WHITE_PAWN]))
				outsidePawn--;
		}
	}
	return outsidePawn;
}

int bishopMobility(Board *pBoard);

int rookMobility(Board *pBoard);

int kingSafety(Board *pBoard);

int controlOfCenter(Board *pBoard);

int control(Board *pBoard, int square) {
	// we count the number of white pieces attacking the square, and subtract the number of black pieces.
	assert(false);
	return 0;
}
