#include "protos.h"
#include "defines.h"
#include "extglobals.h"


// bool pawnPromotion(Board *pBoard, BitBoard pawnLocation);
// 
// BitBoard bishopTimidBB(Board *pBoard, BitBoard bishopLocation);
// BitBoard kingTimidBB(Board *pBoard, BitBoard kingLocation);
// BitBoard pawnTimidBB(Board *pBoard, BitBoard pawnLocation);
// BitBoard rookTimidBB(Board *pBoard, BitBoard rookLocation);
// 
// BitBoard bishopCaptureBB(Board *pBoard, BitBoard bishopLocation);
// BitBoard kingCaptureBB(Board *pBoard, BitBoard kingLocation);
// BitBoard pawnCaptureBB(Board *pBoard, BitBoard pawnLocation);
// BitBoard rookCaptureBB(Board *pBoard, BitBoard rookLocation);
// 
// BitBoard bishopMoveBB(Board *pBoard, BitBoard bishopLocation);
// BitBoard kingMoveBB(Board *pBoard, BitBoard kingLocation);
// BitBoard pawnMoveBB(Board *pBoard, BitBoard pawnLocation);
// BitBoard rookMoveBB(Board *pBoard, BitBoard rookLocation);

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
		break;make
		case B:
			return knightMove[knightIndex] & ~pBoard->position.black.pieces;
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
				return oneStep | pawnDoubleWhite[pawnIndex] & ~pBoard->position.occupied;
			} else {
				return oneStep;
			}
		break;
		case B:
			oneStep = pawnMoveBlack[pawnIndex] & ~pBoard->position.occupied;
			if(oneStep) {
				return oneStep | pawnDoubleBlack[pawnIndex] & ~pBoard->position.occupied;
			} else {
				return oneStep;
			}
		break;
	}
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
}

BitBoard pawnMoveBB(Board *pBoard, UCHAR pawnIndex, int side) {
	return pawnCaptureBB(pBoard, pawnIndex, side) | pawnTimidBB(pBoard, pawnIndex, side);
}