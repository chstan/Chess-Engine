#include "protos.h"
#include "move.h"
#include "defines.h"

// void generateCheck(Board *pBoard);
 
// BitBoard attacks(Board *pBoard, int attackeeIndex);
// BitBoard checks(Board *pBoard, int kingIndex);

// void extractMoves(Board *pBoard);

void generateAgnostic(Board *pBoard, int color, BitBoard currentPieces, int piece, MoveSet *pMoves, BitBoard (*moveGen)(Board *pBoard, UCHAR origin, int color)) {
	int origin = 0, shift = 0, destination = 0;
	Move currentMove = 0;
	BitBoard generatedMoves = 0;
	
	while(currentPieces) {
		shift = LSB(currentPieces);
		currentPieces >>= shift;
		origin += shift;
		
		generatedMoves = moveGen(pBoard, origin, color);
		int i = 0;
		destination = 0;
		while(generatedMoves) {
			i = LSB(generatedMoves);
			generatedMoves >>= i;
			destination += i;
			currentMove = move(0, piece, origin, destination);
			writeMove(pMoves, currentMove);
		}
	}
	
	return;
}

void generateTimid(Board *pBoard, MoveSet *pMoves) {
	BitBoard currentPieces = 0;
	int color = pBoard->info.nextMove;
	int piece;
	
	//pawn
	currentPieces = (color) ?
				pBoard->position.black.pawn
			: pBoard->position.white.pawn;
	piece = (color) ?
				BLACK_PAWN
			: WHITE_PAWN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, pawnTimidBB);
	
	//bishop
	currentPieces = (color) ?
				pBoard->position.black.bishop
			: pBoard->position.white.bishop;
	piece = (color) ?
				BLACK_BISHOP
			: WHITE_BISHOP;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, bishopTimidBB);
	
	//knight
	currentPieces = (color) ?
				pBoard->position.black.knight
			: pBoard->position.white.knight;
	piece = (color) ?
				BLACK_PAWN
			: WHITE_PAWN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, knightTimidBB);
	
	//rook
	currentPieces = (color) ?
				pBoard->position.black.rook
			: pBoard->position.white.rook;
	piece = (color) ?
				BLACK_ROOK
			: WHITE_ROOK;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, rookTimidBB);
	
	//queen
	currentPieces = (color) ?
				pBoard->position.black.queen
			: pBoard->position.white.queen;
	piece = (color) ?
				BLACK_QUEEN
			: WHITE_QUEEN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, queenTimidBB);
	
	//king
	currentPieces = (color) ?
				pBoard->position.black.king
			: pBoard->position.white.king;
	piece = (color) ?
				BLACK_KING
			: WHITE_KING;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, kingTimidBB);
	
	return;
}

void generateCapture(Board *pBoard, MoveSet *pMoves) {
	BitBoard currentPieces = 0;
	int color = pBoard->info.nextMove;
	int piece;
	
	//pawn
	currentPieces = (color) ?
				pBoard->position.black.pawn
			: pBoard->position.white.pawn;
	piece = (color) ?
				BLACK_PAWN
			: WHITE_PAWN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, pawnCaptureBB);
	
	//bishop
	currentPieces = (color) ?
				pBoard->position.black.bishop
			: pBoard->position.white.bishop;
	piece = (color) ?
				BLACK_BISHOP
			: WHITE_BISHOP;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, bishopCaptureBB);
	
	//knight
	currentPieces = (color) ?
				pBoard->position.black.knight
			: pBoard->position.white.knight;
	piece = (color) ?
				BLACK_PAWN
			: WHITE_PAWN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, knightCaptureBB);
	
	//rook
	currentPieces = (color) ?
				pBoard->position.black.rook
			: pBoard->position.white.rook;
	piece = (color) ?
				BLACK_ROOK
			: WHITE_ROOK;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, rookCaptureBB);
	
	//queen
	currentPieces = (color) ?
				pBoard->position.black.queen
			: pBoard->position.white.queen;
	piece = (color) ?
				BLACK_QUEEN
			: WHITE_QUEEN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, queenCaptureBB);
	
	//king
	currentPieces = (color) ?
				pBoard->position.black.king
			: pBoard->position.white.king;
	piece = (color) ?
				BLACK_KING
			: WHITE_KING;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, kingCaptureBB);
	
	return;
}

void generateMove(Board *pBoard, MoveSet *pMoves) {
	BitBoard currentPieces = 0;
	int color = pBoard->info.nextMove;
	int piece;
	
	//pawn
	currentPieces = (color) ?
				pBoard->position.black.pawn
			: pBoard->position.white.pawn;
	piece = (color) ?
				BLACK_PAWN
			: WHITE_PAWN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, pawnMoveBB);
	
	//bishop
	currentPieces = (color) ?
				pBoard->position.black.bishop
			: pBoard->position.white.bishop;
	piece = (color) ?
				BLACK_BISHOP
			: WHITE_BISHOP;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, bishopMoveBB);
	
	//knight
	currentPieces = (color) ?
				pBoard->position.black.knight
			: pBoard->position.white.knight;
	piece = (color) ?
				BLACK_PAWN
			: WHITE_PAWN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, knightMoveBB);
	
	//rook
	currentPieces = (color) ?
				pBoard->position.black.rook
			: pBoard->position.white.rook;
	piece = (color) ?
				BLACK_ROOK
			: WHITE_ROOK;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, rookMoveBB);
	
	//queen
	currentPieces = (color) ?
				pBoard->position.black.queen
			: pBoard->position.white.queen;
	piece = (color) ?
				BLACK_QUEEN
			: WHITE_QUEEN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, queenMoveBB);
	
	//king
	currentPieces = (color) ?
				pBoard->position.black.king
			: pBoard->position.white.king;
	piece = (color) ?
				BLACK_KING
			: WHITE_KING;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, kingMoveBB);
	return;
}

//===============UNFINISHED===================

void generateCheck(Board *pBoard, MoveSet *pMoves) {
	
	return;
}

BitBoard attacks(Board *pBoard, int attackeeIndex, int side) {
	
	return 0;
}

BitBoard checks(Board *pBoard, int side) {
	
	return 0;
}

void initializeMoveSet(Board *pBoard, MoveSet *pMoves) {
	return;
}
 
Move next(MoveSet *pMoves) {
	return 0;
}

void writeMove(MoveSet *pMoves, Move m) {
	return;
}