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

//---------------QUEEN----------------
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