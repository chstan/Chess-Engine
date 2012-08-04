#include <assert.h>
#include <stdio.h>

#include "movegen.h"
#include "movegenhelpers.h"
#include "move.h"
#include "../bit.h"
#include "../defines.h"

MoveGenCB moveCB[] = {
	NULL, 
	pawnMoveBB,
	kingMoveBB,
	knightMoveBB,
	NULL,
	bishopMoveBB,
	rookMoveBB,
	queenMoveBB,
	NULL, 
	pawnMoveBB,
	kingMoveBB,
	knightMoveBB,
	NULL,
	bishopMoveBB,
	rookMoveBB,
	queenMoveBB
};

MoveGenCB timidCB[] = {
	NULL, 
	pawnTimidBB,
	kingTimidBB,
	knightTimidBB,
	NULL,
	bishopTimidBB,
	rookTimidBB,
	queenTimidBB,
	NULL, 
	pawnTimidBB,
	kingTimidBB,
	knightTimidBB,
	NULL,
	bishopTimidBB,
	rookTimidBB,
	queenTimidBB
};

MoveGenCB captureCB[] = {
	NULL, 
	pawnCaptureBB,
	kingCaptureBB,
	knightCaptureBB,
	NULL,
	bishopCaptureBB,
	rookCaptureBB,
	queenCaptureBB,
	NULL, 
	pawnCaptureBB,
	kingCaptureBB,
	knightCaptureBB,
	NULL,
	bishopCaptureBB,
	rookCaptureBB,
	queenCaptureBB
};

void generateAgnostic(Board *pBoard, int color, BitBoard currentPieces, 
int piece, MoveSet *pMoves, BitBoard (*moveGen)(Board *pBoard, UCHAR origin, int color)) {
	int origin = -1, shift = 0, destination = 0;
	int count = 0;
	Move currentMove = 0;
	BitBoard generatedMoves = 0;
	while(currentPieces) {
		shift = LSB(currentPieces)+1;
		currentPieces >>= shift;
		origin += shift;
		generatedMoves = moveGen(pBoard, origin, color);
		//if(generatedMoves) printBitMap(generatedMoves);
		int i = 0;
		destination = -1;
		while(generatedMoves) {
			count += 1;
			i = LSB(generatedMoves)+1;
			generatedMoves >>= i;
			destination += i;
			
			// we need to determine other information about the move!
			// we should factor this out into a helper method, extractMove
			currentMove = move(0, piece, origin, destination);
			writeMove(pMoves, currentMove);
		}
	}
	
	return;
}

void generateTimid(Board *pBoard, MoveSet *pMoves) {
	BitBoard currentPieces = 0;
	int color = pBoard->info.toPlay;
	int piece, endpiece;

	if (color == BLACK) {
		piece = BP;
		endpiece = BQ;
	} else {
		piece = WP;
    endpiece = WQ;
	}

	for (; piece <= endpiece; piece++) {
		if (!isPiece(piece)) continue;
		currentPieces = pBoard->position.pieceBB[piece];
		generateAgnostic(pBoard, color, currentPieces, piece, pMoves, timidCB[piece]);
	}
	
	return;
}

void generateCapture(Board *pBoard, MoveSet *pMoves) {
	BitBoard currentPieces = 0;
	int color = pBoard->info.toPlay;
	int piece, endpiece;

	if (color == BLACK) {
		piece = BP;
		endpiece = BQ;
	} else {
		piece = WP;
		endpiece = WQ;
	}

	for (; piece <= endpiece; piece++) {
		if (!isPiece(piece)) continue;
		currentPieces = pBoard->position.pieceBB[piece];
		generateAgnostic(pBoard, color, currentPieces, piece, pMoves, captureCB[piece]);
	}
	
	return;
}

void generateMove(Board *pBoard, MoveSet *pMoves) {
	BitBoard currentPieces = 0;
	int color = pBoard->info.toPlay;
	int piece, endpiece;

	if (color == BLACK) {
		piece = BP;
		endpiece = BQ;
	} else {
		piece = WP;
		endpiece = WQ;
	}

	for (; piece <= endpiece; piece++) {
		if (!isPiece(piece)) continue;
		currentPieces = pBoard->position.pieceBB[piece];
		generateAgnostic(pBoard, color, currentPieces, piece, pMoves, moveCB[piece]);
	}
	
	return;
}

BitBoard generatePieceAttacks(Board *pBoard, int color, BitBoard currentPieces, 
int piece, BitBoard (*moveGen)(Board *pBoard, UCHAR origin, int color)) {
	int origin = -1, shift = 0;
	BitBoard generatedMoves = 0;
	while(currentPieces) {
		shift = LSB(currentPieces)+1;
		currentPieces >>= shift;
		origin += shift;
		generatedMoves |= moveGen(pBoard, origin, color);
	}
	return generatedMoves;
}

BitBoard generateAllAttacks(Board *pBoard) {
	BitBoard attacks;
	BitBoard currentPieces = 0;
	int color = pBoard->info.toPlay;
	int piece, endpiece;

	if (color == BLACK) {
		piece = WP;
		endpiece = WQ;
	} else {
		piece = BP;
		endpiece = BQ;
	}

	for (; piece <= endpiece; piece++) {
		if (!isPiece(piece)) continue;
		currentPieces = pBoard->position.pieceBB[piece];
		attacks |= generatePieceAttacks(pBoard, color, currentPieces, piece, captureCB[piece]);
	}
	
	return attacks;
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
}

void initializeMoveSetQuiet(Board *pBoard, MoveSet *pMoves) {
	generateCapture(pBoard, pMoves);
	generateTimid(pBoard, pMoves);
}

void resetMoveSet(MoveSet *pMoves) {
	pMoves->moveIter = 0;
	pMoves->currentMoveIndex = 0;
	pMoves->timidIndex = 0;
	pMoves->totalMoves = 0;
	
	pMoves->killerIter = 0;
	pMoves->totalKillers = 0;
	pMoves->currentKillerIndex = 0;
}

Move nextMove(MoveSet *pMoves) {
	assert(pMoves->moveIter < pMoves->totalMoves);
	return pMoves->moveList[pMoves->moveIter++];
}

void writeMove(MoveSet *pMoves, Move m) {
	assert(pMoves->currentMoveIndex + 1 < MAX_MOVES_PER_PLY);
	pMoves->totalMoves++;
	pMoves->moveList[pMoves->currentMoveIndex++] = m;
	if(capturedPiece(m)) pMoves->timidIndex++;
}

void writeKiller(MoveSet *pMoves, Move killer) {
	assert(pMoves->currentKillerIndex + 1 < MAX_KILLERS_PER_PLY);
	pMoves->totalKillers++;
	pMoves->killerList[pMoves->currentKillerIndex++] = killer;
}
