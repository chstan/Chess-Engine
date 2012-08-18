#include "evalhelpers.h"
#include "../defines.h"
#include "../extglobals.h"

int doubledPawns(Board *pBoard) {
	int doubledPawns = 0;
	for(int currentFile = 0; currentFile < 8; currentFile++) {
		if(bitCount(pBoard->position.pieceBB[WHITE_PAWN] & fileBB[currentFile]) == 2) doubledPawns++;
		if(bitCount(pBoard->position.pieceBB[BLACK_PAWN] & fileBB[currentFile]) == 2) doubledPawns--;
	}
	return doubledPawns;
}

int tripledPawns(Board *pBoard) {
	int tripledPawns = 0;
	for(int currentFile = 0; currentFile < 8; currentFile++) {
		if(bitCount(pBoard->position.pieceBB[WHITE_PAWN] & fileBB[currentFile]) == 3) tripledPawns++;
		if(bitCount(pBoard->position.pieceBB[BLACK_PAWN] & fileBB[currentFile]) == 3) tripledPawns--;
	}
	return tripledPawns;
}

float bishopMobility(Board *pBoard);

float rookMobility(Board *pBoard);

float kingSafety(Board *pBoard);

float controlOfCenter(Board *pBoard);

int control(Board *pBoard, int square) {
	// we count the number of white pieces attacking the square, and subtract the number of black pieces.
}