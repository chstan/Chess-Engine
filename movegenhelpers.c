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
	return bishopTimidBB(pBoard, queenLocation, side)
			 | rookTimidBB(pBoard, queenLocation, side);
}

BitBoard queenCaptureBB(Board *pBoard, UCHAR queenIndex, int side) {
	return bishopCaptureBB(pBoard, queenLocation, side)
			 | rookCaptureBB(pBoard, queenLocation, side);
}

BitBoard queenMoveBB(Board *pBoard, UCHAR queenIndex, int side) {
	return bishopMoveBB(pBoard, queenLocation, side)
			 | rookMoveBB(pBoard, queenLocation, side);
}

//---------------KNIGHT----------------
BitBoard knightTimidBB(Board *pBoard, UCHAR knightIndex, int side) {
	return knightMoves[knightIndex] & ~pBoard->position.occupied;
}

BitBoard knightCaptureBB(Board *pBoard, UCHAR knightIndex, int side) {
	switch(side) {
		case W:
			return knightMoves[knightIndex] & pBoard->position.black.pieces;
		break;
		case B:
			return knightMoves[knightIndex] & pBoard->position.white.pieces;
		break;
	}
}

BitBoard knightMoveBB(Board *pBoard, UCHAR knightIndex, int side) {
	switch(side) {
		case W:
			return knightMoves[knightIndex] & ~pBoard->position.white.pieces;
		break;
		case B:
			return knightMoves[knightIndex] & ~pBoard->position.black.pieces;
		break;
	}
}