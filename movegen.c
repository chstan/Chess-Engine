#include "protos.h"
#include "move.h"
#include "defines.h"

// void generateTimid(Board *pBoard);
// void generateCapture(Board *pBoard);
// void generateCheck(Board *pBoard);
// void generateMove(Board *pBoard);
 
// BitBoard attacks(Board *pBoard, int attackeeIndex);
// BitBoard checks(Board *pBoard, int kingIndex);

// void extractMoves(Board *pBoard);

void generateTimid(Board *pBoard, MoveSet *pMoves) {
	BitBoard currentPieces = 0;
	int color = pBoard->nextMove;
	int origin = 0, shift = 0, destination = 0;
	Move currentMove;
	BitBoard generatedMoves = 0;
	
	
	//pawn
	currentPieces = (color) ?
				pBoard->position.black.pawn
			: pBoard->position.white.pawn;
	
	while(currentPieces) {
		shift = LSB(currentPieces);
		currentPieces >>= shift;
		origin += shift
		
		generatedMoves = pawnTimidBB(pBoard, origin, color);
		int i = 0;
		destination = 0;
		while(generatedMoves) {
			i = LSB(generatedMoves);
			generatedMoves >>= i;
			destination += i;
			currentMove = move(0, color ? BLACK_PAWN : WHITE_PAWN, origin, destination);
			writeMove(pMoves, currentMove);
		}
	}
	
	//bishop
	currentPieces = (color) ?
				pBoard->position.black.bishop
			: pBoard->position.white.bishop;
			
	while(currentPieces) {
		shift = LSB(currentPieces);
		currentPieces >>= shift;
		origin += shift
		
		generatedMoves = bishopTimidBB(pBoard, origin, color);
		int i = 0;
		destination = 0;
		while(generatedMoves) {
			i = LSB(generatedMoves);
			generatedMoves >>= i;
			destination += i;
			currentMove = move(0, color ? BLACK_BISHOP : WHITE_BISHOP, origin, destination);
			writeMove(pMoves, currentMove);
		}
	}
	
	//knight
	currentPieces = (color) ?
				pBoard->position.black.knight
			: pBoard->position.white.knight;
			
	while(currentPieces) {
		shift = LSB(currentPieces);
		currentPieces >>= shift;
		origin += shift
		
		generatedMoves = knightTimidBB(pBoard, origin, color);
		int i = 0;
		destination = 0;
		while(generatedMoves) {
			i = LSB(generatedMoves);
			generatedMoves >>= i;
			destination += i;
			currentMove = move(0, color ? BLACK_KNIGHT : WHITE_KNIGHT, origin, destination);
			writeMove(pMoves, currentMove);
		}
	}
	
	//rook
	currentPieces = (color) ?
				pBoard->position.black.rook
			: pBoard->position.white.rook;
			
	while(currentPieces) {
		shift = LSB(currentPieces);
		currentPieces >>= shift;
		origin += shift
		
		generatedMoves = rookTimidBB(pBoard, origin, color);
		int i = 0;
		destination = 0;
		while(generatedMoves) {
			i = LSB(generatedMoves);
			generatedMoves >>= i;
			destination += i;
			currentMove = move(0, color ? BLACK_ROOK : WHITE_ROOK, origin, destination);
			writeMove(pMoves, currentMove);
		}
	}
	
	//queen
	currentPieces = (color) ?
				pBoard->position.black.queen
			: pBoard->position.white.queen;
			
	while(currentPieces) {
		shift = LSB(currentPieces);
		currentPieces >>= shift;
		origin += shift
		
		generatedMoves = queenTimidBB(pBoard, origin, color);
		int i = 0;
		destination = 0;
		while(generatedMoves) {
			i = LSB(generatedMoves);
			generatedMoves >>= i;
			destination += i;
			currentMove = move(0, color ? BLACK_QUEEN : WHITE_QUEEN, origin, destination);
			writeMove(pMoves, currentMove);
		}
	}
	
	//king
	currentPieces = (color) ?
				pBoard->position.black.king
			: pBoard->position.white.king;
			
	while(currentPieces) {
		shift = LSB(currentPieces);
		currentPieces >>= shift;
		origin += shift
		
		generatedMoves = kingTimidBB(pBoard, origin, color);
		int i = 0;
		destination = 0;
		while(generatedMoves) {
			i = LSB(generatedMoves);
			generatedMoves >>= i;
			destination += i;
			currentMove = move(0, color ? BLACK_KING : WHITE_KING, origin, destination);
			writeMove(pMoves, currentMove);
		}
	}
	
	return;
}

void generateCapture(Board *pBoard, MoveSet *pMoves) {
	
	return;
}

void generateCheck(Board *pBoard, MoveSet *pMoves) {
	
	return;
}

void generateMove(Board *pBoard, MoveSet *pMoves) {
	
	return;
}

void initializeMoveSet(Board *pBoard, MoveSet *pMoves) {
	
	return;
}

BitBoard attacks(Board *pBoard, int attackeeIndex, int side) {
	
	return 0;
}

BitBoard checks(Board *pBoard, int side) {
	
	return 0;
}