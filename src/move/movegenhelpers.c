
#include "movegenhelpers.h"
#include "../defines.h"
#include "../extglobals.h"

//----------------ROOK-----------------

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

BitBoard rookCaptureBB(Board *pBoard, UCHAR rookIndex, int side) {
	BitBoard validMoves = 0;
	// UP
	BitBoard currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 8;
	}
	if(currentIndex & (side ? pBoard->position.whiteOccupied : pBoard->position.blackOccupied)) {
		validMoves |= currentIndex;
	}
	// DOWN
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 8;
	}
	if(currentIndex & (side ? pBoard->position.whiteOccupied : pBoard->position.blackOccupied)) {
		validMoves |= currentIndex;
	}
	// RIGHT
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 1;
	}
	if(currentIndex & (side ? pBoard->position.whiteOccupied : pBoard->position.blackOccupied)) {
		validMoves |= currentIndex;
	}
	// LEFT
	currentIndex = BITSET[rookIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 1;
	}
	if(currentIndex & (side ? pBoard->position.whiteOccupied : pBoard->position.blackOccupied)) {
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

BitBoard bishopCaptureBB(Board *pBoard, UCHAR bishopIndex, int side) {
	BitBoard validMoves = 0;
	// UL
	BitBoard currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 9;
	}
	if(currentIndex & (side ? pBoard->position.whiteOccupied : pBoard->position.blackOccupied)) {
		validMoves |= currentIndex;
	}
	// UR
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex <<= 7;
	}
	if(currentIndex & (side ? pBoard->position.whiteOccupied : pBoard->position.blackOccupied)) {
		validMoves |= currentIndex;
	}
	// DL
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 9;
	}
	if(currentIndex & (side ? pBoard->position.whiteOccupied : pBoard->position.blackOccupied)) {
		validMoves |= currentIndex;
	}
	// DR
	currentIndex = BITSET[bishopIndex];
	while(currentIndex & ~(pBoard->position.occupied)) {
		currentIndex >>= 7;
	}
	if(currentIndex & (side ? pBoard->position.whiteOccupied : pBoard->position.blackOccupied)) {
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
