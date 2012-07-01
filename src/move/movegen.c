#include "protos.h"
#include "move.h"
#include "defines.h"
#include <assert.h>
#include <stdio.h>


// void generateCheck(Board *pBoard);
 
// BitBoard attacks(Board *pBoard, int attackeeIndex);
// BitBoard checks(Board *pBoard, int kingIndex);

// void extractMoves(Board *pBoard);

void generateAgnostic(Board *pBoard, int color, BitBoard currentPieces, 
int piece, MoveSet *pMoves, BitBoard (*moveGen)(Board *pBoard, UCHAR origin, int color)) {
	int origin = -1, shift = 0, destination = 0;
	int count = 0;
	Move currentMove = 0;
	BitBoard generatedMoves = 0;
	//printf("Entering loop.\n");
	while(currentPieces) {
		shift = LSB(currentPieces)+1;
		currentPieces >>= shift;
		origin += shift;
		//printf("Origin: %d\t Count: %d\t Current Pieces:\n", origin, count);
		//printBitMap(currentPieces);
		generatedMoves = moveGen(pBoard, origin, color);
		if(generatedMoves) printBitMap(generatedMoves);
		int i = 0;
		destination = 0;
		while(generatedMoves) {
			count += 1;
			i = LSB(generatedMoves)+1;
			generatedMoves >>= i;
			destination += i;
			currentMove = move(0, piece, origin, destination);
			writeMove(pMoves, currentMove);
			//printMove(currentMove);
		}
	}
	
	return;
}

void generateTimid(Board *pBoard, MoveSet *pMoves) {
	BitBoard currentPieces = 0;
	int color = pBoard->info.nextMove;
	int piece;
	
	//pawn
	currentPieces = (color == BLACK) ?
				pBoard->position.black.pawn
			: pBoard->position.white.pawn;
	piece = (color) ?
				BLACK_PAWN
			: WHITE_PAWN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, pawnTimidBB);
	
	//bishop
	currentPieces = (color == BLACK) ?
				pBoard->position.black.bishop
			: pBoard->position.white.bishop;
	piece = (color) ?
				BLACK_BISHOP
			: WHITE_BISHOP;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, bishopTimidBB);
	
	//knight
	currentPieces = (color == BLACK) ?
				pBoard->position.black.knight
			: pBoard->position.white.knight;
	piece = (color) ?
				BLACK_PAWN
			: WHITE_PAWN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, knightTimidBB);
	
	//rook
	currentPieces = (color == BLACK) ?
				pBoard->position.black.rook
			: pBoard->position.white.rook;
	piece = (color) ?
				BLACK_ROOK
			: WHITE_ROOK;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, rookTimidBB);
	
	//queen
	currentPieces = (color == BLACK) ?
				pBoard->position.black.queen
			: pBoard->position.white.queen;
	piece = (color) ?
				BLACK_QUEEN
			: WHITE_QUEEN;
			
	generateAgnostic(pBoard, color, currentPieces, piece, pMoves, queenTimidBB);
	
	//king
	currentPieces = (color == BLACK) ?
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
	resetMoveSet(pMoves);
	printf("About to generate moves.\n");
	generateCapture(pBoard, pMoves);
	generateTimid(pBoard, pMoves);
	printf("Generated moves.\n");
	while(pMoves->moveIter < pMoves->totalMoves)
		printMove(nextMove(pMoves));
	return;
}

/**
 *\brief resetMoveSet
 *
 *	resetMoveSet is an auxilliary function
 *  which effectively clears the state of the
 *  moveSet passed in as the parameter.
 *
 *\param  Pointer to the moveset
 *\return (void)
**/
void resetMoveSet(MoveSet *pMoves) {
	pMoves->moveIter = 0;
	pMoves->currentMoveIndex = 0;
	pMoves->timidIndex = 0;
	pMoves->totalMoves = 0;
	
	pMoves->killerIter = 0;
	pMoves->totalKillers = 0;
	pMoves->currentKillerIndex = 0;
	return;
}

/**
 *\brief nextMove
 *
 *	nextMove returns the next unread move from the
 *  moveSet and updates the iterator to point to the
 *  next spot in the array. It also asserts that you are not
 *  trying to read an unwritten move from the moveSet.
 *
 *\param  Pointer to the moveSet
 *\return Move that was read from the moveSet
**/
Move nextMove(MoveSet *pMoves) {
	assert(pMoves->moveIter < pMoves->totalMoves);
	return pMoves->moveList[pMoves->moveIter++];
}

/**
 *\brief writeMove
 *
 *	writeMove adds a non-killer move to a moveSet
 *  and leaves the moveSet in a consistent state, updating
 *  iterators as necessary.
 *
 *\param  Pointer to the moveSet
 *\param  Move to be written
 *\return (void)
**/
void writeMove(MoveSet *pMoves, Move m) {
	assert(pMoves->currentMoveIndex + 1 < MAX_MOVES_PER_PLY);
	pMoves->totalMoves++;
	pMoves->moveList[pMoves->currentMoveIndex++] = m;
	if(capturedPiece(m)) pMoves->timidIndex++;
	return;
}

/**
 *\brief writeKiller
 *
 *	writeKiller adds a killer move to a moveSet
 *  and leaves the moveSet in a consistent state, updating
 *  iterators as necessary.
 *
 *\param  Pointer to the moveSet
 *\param  Killer move to be written
 *\return (void)
**/
void writeKiller(MoveSet *pMoves, Move killer) {
	assert(pMoves->currentKillerIndex + 1 < MAX_KILLERS_PER_PLY);
	pMoves->totalKillers++;
	pMoves->killerList[pMoves->currentKillerIndex++] = killer;
	return;
}
