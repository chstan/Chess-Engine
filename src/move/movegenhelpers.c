#include "protos.h"
#include "defines.h"
#include "extglobals.h"

//----------------ROOK-----------------

/* Function: rookTimidBB
 * Finds the valid moves for a rook at rookIndex which
 * does not capture any piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard rookTimidBB(Board *pBoard, UCHAR rookIndex, int side) {
	BitBoard validMoves = 0;
	// UP
	BitBoard currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 8;
		validMoves |= currentIndex;
	}
	// DOWN
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 8;
		validMoves |= currentIndex;
	}
	// RIGHT
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 1;
		validMoves |= currentIndex;
	}
	// LEFT
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 1;
		validMoves |= currentIndex;
	}
	return validMoves;
}

/* Function: rookCaptureBB
 * Finds the valid moves for a rook at rookIndex which
 * do capture an enemy piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard rookCaptureBB(Board *pBoard, UCHAR rookIndex, int side) {
	BitBoard validMoves = 0;
	// UP
	BitBoard currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 8;
	}
	if(currentIndex & (side ? pBoard->position.white.pieces : pBoard->position.black.pieces)) {
		validMoves |= currentIndex;
	}
	// DOWN
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 8;
	}
	if(currentIndex & (side ? pBoard->position.white.pieces : pBoard->position.black.pieces)) {
		validMoves |= currentIndex;
	}
	// RIGHT
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 1;
	}
	if(currentIndex & (side ? pBoard->position.white.pieces : pBoard->position.black.pieces)) {
		validMoves |= currentIndex;
	}
	// LEFT
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 1;
	}
	if(currentIndex & (side ? pBoard->position.white.pieces : pBoard->position.black.pieces)) {
		validMoves |= currentIndex;
	}
	return validMoves;
}

/* Function: rookMoveBB
 * Finds the valid moves for a rook at rookIndex which can
 * but does not have to capture a piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard rookMoveBB(Board *pBoard, UCHAR rookIndex, int side) {
	return rookTimidBB(pBoard, rookIndex, side)
			 | rookCaptureBB(pBoard, rookIndex, side);
}

//---------------BISHOP----------------

/* Function: bishopTimidBB
 * Finds the valid moves for a bishop at bishopIndex which
 * does not capture any piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard bishopTimidBB(Board *pBoard, UCHAR bishopIndex, int side) {
	BitBoard validMoves = 0;
	// UL
	BitBoard currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 9;
		validMoves |= currentIndex;
	}
	// UR
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 7;
		validMoves |= currentIndex;
	}
	// DL
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 9;
		validMoves |= currentIndex;
	}
	// DR
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 7;
		validMoves |= currentIndex;
	}
	return validMoves;
}

/* Function: bishopCaptureBB
 * Finds the valid moves for a bishop at bishopIndex which
 * do capture an enemy piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard bishopCaptureBB(Board *pBoard, UCHAR bishopIndex, int side) {
	BitBoard validMoves = 0;
	// UL
	BitBoard currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 9;
	}
	if(currentIndex & (side ? pBoard->position.white.pieces : pBoard->position.black.pieces)) {
		validMoves |= currentIndex;
	}
	// UR
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 7;
	}
	if(currentIndex & (side ? pBoard->position.white.pieces : pBoard->position.black.pieces)) {
		validMoves |= currentIndex;
	}
	// DL
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 9;
	}
	if(currentIndex & (side ? pBoard->position.white.pieces : pBoard->position.black.pieces)) {
		validMoves |= currentIndex;
	}
	// DR
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 7;
	}
	if(currentIndex & (side ? pBoard->position.white.pieces : pBoard->position.black.pieces)) {
		validMoves |= currentIndex;
	}
	return validMoves;
}

/* Function: bishopMoveBB
 * Finds the valid moves for a bishop at bishopIndex which can
 * but does not have to capture a piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard bishopMoveBB(Board *pBoard, UCHAR bishopIndex, int side) {
	return bishopTimidBB(pBoard, bishopIndex, side)
			 | bishopCaptureBB(pBoard, bishopIndex, side);
}

//----------------QUEEN----------------
BitBoard queenTimidBB(Board *pBoard, UCHAR queenIndex, int side) {
	return bishopTimidBB(pBoard, queenIndex, side)
			 | rookTimidBB(pBoard, queenIndex, side);
}

BitBoard queenCaptureBB(Board *pBoard, UCHAR queenIndex, int side) {
	return bishopCaptureBB(pBoard, queenIndex, side)
			 | rookCaptureBB(pBoard, queenIndex, side);
}

BitBoard queenMoveBB(Board *pBoard, UCHAR queenIndex, int side) {
	return bishopMoveBB(pBoard, queenIndex, side)
			 | rookMoveBB(pBoard, queenIndex, side);
}

//---------------KNIGHT----------------
BitBoard knightTimidBB(Board *pBoard, UCHAR knightIndex, int side) {
	return knightMove[knightIndex] & ~pBoard->position.occupied;
}

BitBoard knightCaptureBB(Board *pBoard, UCHAR knightIndex, int side) {
	switch(side) {
		case W:
			return knightMove[knightIndex] & pBoard->position.black.pieces;
		break;
		case B:
			return knightMove[knightIndex] & pBoard->position.white.pieces;
		break;
	}
	return 0;
}

BitBoard knightMoveBB(Board *pBoard, UCHAR knightIndex, int side) {
	switch(side) {
		case W:
			return knightMove[knightIndex] & ~pBoard->position.white.pieces;
		break;
		case B:
			return knightMove[knightIndex] & ~pBoard->position.black.pieces;
		break;
	}
	return 0;
}

//----------------KING-----------------
BitBoard kingTimidBB(Board *pBoard, UCHAR kingIndex, int side) {
	return kingMove[kingIndex] & ~pBoard->position.occupied;
}

BitBoard kingCaptureBB(Board *pBoard, UCHAR kingIndex, int side) {
	switch(side) {
		case W:
			return kingMove[kingIndex] & pBoard->position.black.pieces;
		break;
		case B:
			return kingMove[kingIndex] & pBoard->position.white.pieces;
		break;
	}
	return 0;
}

BitBoard kingMoveBB(Board *pBoard, UCHAR kingIndex, int side) {
	switch(side) {
		case W:
			return kingMove[kingIndex] & ~pBoard->position.white.pieces;
		break;
		case B:
			return kingMove[kingIndex] & ~pBoard->position.black.pieces;
		break;
	}
	return 0;
}

//----------------PAWN-----------------
BitBoard pawnTimidBB(Board *pBoard, UCHAR pawnIndex, int side) {
	BitBoard oneStep;
	switch(side) {
		case W:
			oneStep = pawnMoveWhite[pawnIndex] & ~pBoard->position.occupied;
			if(oneStep) {
				return oneStep | (pawnDoubleWhite[pawnIndex] & ~pBoard->position.occupied);
			} else {
				return oneStep;
			}
		break;
		case B:
			oneStep = pawnMoveBlack[pawnIndex] & ~pBoard->position.occupied;
			if(oneStep) {
				return oneStep | (pawnDoubleBlack[pawnIndex] & ~pBoard->position.occupied);
			} else {
				return oneStep;
			}
		break;
	}
	return 0;
}

BitBoard pawnPromotionBB(Board *pBoard, UCHAR pawnIndex, int side) {
	switch(side) {
		case W:
			return pawnPromotionWhite[pawnIndex] & ~pBoard->position.occupied;
		break;
		case B:
			return pawnPromotionBlack[pawnIndex] & ~pBoard->position.occupied;
		break;
	}
	return 0;
}

BitBoard pawnCaptureBB(Board *pBoard, UCHAR pawnIndex, int side) {
	switch(side) {
		case W:
			return pawnCaptureWhite[pawnIndex] & pBoard->position.black.pieces;
		break;
		case B:
			return pawnCaptureBlack[pawnIndex] & pBoard->position.white.pieces;
		break;
	}
	return 0;
}

BitBoard pawnPromotionCaptureBB(Board *pBoard, UCHAR pawnIndex, int side) {
	switch(side) {
		case W:
			return pawnPromotionCaptureWhite[pawnIndex] & pBoard->position.black.pieces;
		break;
		case B:
			return pawnPromotionCaptureBlack[pawnIndex] & pBoard->position.white.pieces;
		break;
	}
	return 0;
}

BitBoard pawnMoveBB(Board *pBoard, UCHAR pawnIndex, int side) {
	return pawnCaptureBB(pBoard, pawnIndex, side) | pawnTimidBB(pBoard, pawnIndex, side);
}
