#include "movegenhelpers.h"
#include "../defines.h"
#include "../extglobals.h"

//----------------ROOK-----------------

BitBoard rookTimidBB(Board *pBoard, UCHAR rookIndex, int side) {
	BitBoard validMoves = 0;
	// UP
	BitBoard currentIndex = BITSET(rookIndex) << 8;
	while(currentIndex & ~(pBoard->position.occupied)) {
		validMoves |= currentIndex;
		currentIndex <<= 8;
	}
	// DOWN
	currentIndex = BITSET(rookIndex) >> 8;
	while(currentIndex & ~(pBoard->position.occupied)) {
		validMoves |= currentIndex;
		currentIndex >>= 8;
	}
	// RIGHT
	currentIndex = BITSET(rookIndex) << 1;
	while(currentIndex & ~(pBoard->position.occupied)) {
		validMoves |= currentIndex;
		currentIndex <<= 1;
	}
	// LEFT
	currentIndex = BITSET(rookIndex) >> 1;
	while(currentIndex & ~(pBoard->position.occupied)) {
		validMoves |= currentIndex;
		currentIndex >>= 1;
	}
	return validMoves;
}

BitBoard rookCaptureBB(Board *pBoard, UCHAR rookIndex, int side) {
	BitBoard validMoves = 0;
	BitBoard takable = (side == BLACK)? pBoard->position.whiteOccupied : pBoard->position.blackOccupied;
	// UP
	BitBoard currentIndex = BITSET(rookIndex) << 8;
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 8;
	}
	if(currentIndex & takable) {
		validMoves |= currentIndex;
	}
	// DOWN
	currentIndex = BITSET(rookIndex) >> 8;
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 8;
	}
	if(currentIndex & takable) {
		validMoves |= currentIndex;
	}
	// RIGHT
	currentIndex = BITSET(rookIndex) << 1;
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 1;
	}
	if(currentIndex & takable) {
		validMoves |= currentIndex;
	}
	// LEFT
	currentIndex = BITSET(rookIndex) >> 1;
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 1;
	}
	if(currentIndex & takable) {
		validMoves |= currentIndex;
	}
	return validMoves;
}

BitBoard rookMoveBB(Board *pBoard, UCHAR rookIndex, int side) {
	return rookTimidBB(pBoard, rookIndex, side)
			 | rookCaptureBB(pBoard, rookIndex, side);
}

//---------------BISHOP----------------

BitBoard bishopTimidBB(Board *pBoard, UCHAR bishopIndex, int side) {
	BitBoard validMoves = 0;
	// UL
	BitBoard currentIndex = BITSET(bishopIndex) << 9;
	while(currentIndex & ~(pBoard->position.occupied)) {
		validMoves |= currentIndex;
		currentIndex <<= 9;
	}
	// UR
	currentIndex = BITSET(bishopIndex) << 7;
	while(currentIndex & ~(pBoard->position.occupied)) {
		validMoves |= currentIndex;
		currentIndex <<= 7;
	}
	// DL
	currentIndex = BITSET(bishopIndex) >> 9;
	while(currentIndex & ~(pBoard->position.occupied)) {
		validMoves |= currentIndex;
		currentIndex >>= 9;
	}
	// DR
	currentIndex = BITSET(bishopIndex) >> 7;
	while(currentIndex & ~(pBoard->position.occupied)) {
		validMoves |= currentIndex;
		currentIndex >>= 7;
	}
	return validMoves;
}

BitBoard bishopCaptureBB(Board *pBoard, UCHAR bishopIndex, int side) {
	BitBoard validMoves = 0;
	BitBoard takable = (side == BLACK)? pBoard->position.whiteOccupied : pBoard->position.blackOccupied;
	// UL
	BitBoard currentIndex = BITSET(bishopIndex) << 9;
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 9;
	}
	if(currentIndex & takable) {
		validMoves |= currentIndex;
	}
	// UR
	currentIndex = BITSET(bishopIndex) << 7;
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 7;
	}
	if(currentIndex & takable) {
		validMoves |= currentIndex;
	}
	// DL
	currentIndex = BITSET(bishopIndex) >> 9;
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 9;
	}
	if(currentIndex & takable) {
		validMoves |= currentIndex;
	}
	// DR
	currentIndex = BITSET(bishopIndex) >> 7;
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 7;
	}
	if(currentIndex & takable) {
		validMoves |= currentIndex;
	}
	return validMoves;
}

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
			return knightMove[knightIndex] & pBoard->position.blackOccupied;
		break;
		case B:
			return knightMove[knightIndex] & pBoard->position.whiteOccupied;
		break;
	}
	return 0;
}

BitBoard knightMoveBB(Board *pBoard, UCHAR knightIndex, int side) {
	switch(side) {
		case W:
			return knightMove[knightIndex] & ~pBoard->position.whiteOccupied;
		break;
		case B:
			return knightMove[knightIndex] & ~pBoard->position.blackOccupied;
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
			return kingMove[kingIndex] & pBoard->position.blackOccupied;
		break;
		case B:
			return kingMove[kingIndex] & pBoard->position.whiteOccupied;
		break;
	}
	return 0;
}

BitBoard kingMoveBB(Board *pBoard, UCHAR kingIndex, int side) {
	switch(side) {
		case W:
			return kingMove[kingIndex] & ~pBoard->position.whiteOccupied;
		break;
		case B:
			return kingMove[kingIndex] & ~pBoard->position.blackOccupied;
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
			}
			return 0;
		break;
		case B:
			oneStep = pawnMoveBlack[pawnIndex] & ~pBoard->position.occupied;
			if(oneStep) {
				return oneStep | (pawnDoubleBlack[pawnIndex] & ~pBoard->position.occupied);
			}
			return 0;
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
			return pawnCaptureWhite[pawnIndex] & pBoard->position.blackOccupied;
		break;
		case B:
			return pawnCaptureBlack[pawnIndex] & pBoard->position.whiteOccupied;
		break;
	}
	return 0;
}

BitBoard pawnPromotionCaptureBB(Board *pBoard, UCHAR pawnIndex, int side) {
	switch(side) {
		case W:
			return pawnPromotionCaptureWhite[pawnIndex] & pBoard->position.blackOccupied;
		break;
		case B:
			return pawnPromotionCaptureBlack[pawnIndex] & pBoard->position.whiteOccupied;
		break;
	}
	return 0;
}

BitBoard pawnMoveBB(Board *pBoard, UCHAR pawnIndex, int side) {
	return pawnCaptureBB(pBoard, pawnIndex, side) | pawnTimidBB(pBoard, pawnIndex, side);
}
