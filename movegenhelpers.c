#include "protos.h"
#include "defines.h"
#include "extglobals.h"

//----------------ROOK-----------------
BitBoard rookTimidBB(Board *pBoard, UCHAR rookIndex, int side) {
	
	return 0;
}

BitBoard rookCaptureBB(Board *pBoard, UCHAR rookIndex, int side) {
	
	return 0;
}

BitBoard rookMoveBB(Board *pBoard, UCHAR rookIndex, int side) {
	
	return 0;
}

//---------------BISHOP----------------
BitBoard bishopTimidBB(Board *pBoard, UCHAR bishopIndex, int side) {
	
	return 0;
}

BitBoard bishopCaptureBB(Board *pBoard, UCHAR bishopIndex, int side) {
	
	return 0;
}

BitBoard bishopMoveBB(Board *pBoard, UCHAR bishopIndex, int side) {
	
	return 0;
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