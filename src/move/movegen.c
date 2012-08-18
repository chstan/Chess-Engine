#include <assert.h>
#include <stdio.h>

#include "movegen.h"
#include "movegenhelpers.h"
#include "move.h"
#include "../bit.h"
#include "../defines.h"

const MoveGenCB moveCB[] = {
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

const MoveGenCB timidCB[] = {
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

const MoveGenCB captureCB[] = {
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

static void debugMoves(MoveSet *pMoves) {
	for(int currentMoveIndex = 0; currentMoveIndex < pMoves->totalMoves; currentMoveIndex++) {
		Move toCheck = pMoves->moveList[currentMoveIndex];
		for(int checkIndex = currentMoveIndex+1; checkIndex < pMoves->totalMoves; checkIndex++) {
			if(toCheck == pMoves->moveList[checkIndex]) {
				printf("Found duplicate move:\n");
				printMove(toCheck);
			}
		}
	}
}

static Move extractMove(int piece, int origin, int destination) {
	int occupant = 0;
	// we assume it's occupied by the appropriate color if this function is called
	if (pBoard->position.occupied & BITSET(destination)) {
		occupant = pBoard->position.square[destination];
	}
	// TODO detect enpassant
	return move(occupant, piece, origin, destination);
}

static void extractEnPassant(Board *pBoard, MoveSet *pMoves) {
	BitBoard generated = enPassantBB(pBoard, pBoard->info.toPlay);
	int color = pBoard->info.toPlay;
	int piece = (color == WHITE) ? WHITE_PAWN : BLACK_PAWN;
	int taken = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
	int origin = -1, shift = 0, destination = 0;
	while(generated) {
		shift = LSB(generated)+1;
		if (shift < 64) generated >>= shift;
		origin += shift;
		int to = pBoard->info.state[pBoard->info.currentMove].enPassantSquare;
		writeMove(pMoves, moveF((color == BLACK) ? 1 : 0, (color == WHITE) ? 1 : 0, 0, 0, 0, taken, piece, origin, to));
	}
	
	return;
}

void generateAgnostic(Board *pBoard, int color, BitBoard currentPieces, 
int piece, MoveSet *pMoves, BitBoard (*moveGen)(Board *pBoard, UCHAR origin, int color)) {
	int origin = -1, shift = 0, destination = 0;
	int count = 0;
	BitBoard generatedMoves = 0;
	while(currentPieces) {
		shift = LSB(currentPieces)+1;
		if (shift < 64) currentPieces >>= shift;
		else currentPieces = 0;
		origin += shift;
		generatedMoves = moveGen(pBoard, origin, color);
		//if(generatedMoves) printBitMap(generatedMoves);
		int i = 0;
		destination = -1;
		while(generatedMoves) {
			count += 1;
			i = LSB(generatedMoves)+1;
			if (i < 64) generatedMoves >>= i;
			else generatedMoves = 0;
			destination += i;
			writeMove(pMoves, extractMove(piece, origin, destination));
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
	
	// handle enPassant
	extractEnPassant(pBoard, pMoves);
	
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
	
	// handle enPassant
	extractEnPassant(pBoard, pMoves);
	
	debugMoves(pMoves);
	return;
}

BitBoard generatePieceAttacks(Board *pBoard, int color, BitBoard currentPieces,
int piece) {
	int origin = -1, shift = 0;
	BitBoard generatedMoves = 0;
	while(currentPieces) {
		shift = LSB(currentPieces)+1;
		if (shift < 64) currentPieces >>= shift;
		else currentPieces = 0;
		origin += shift;
		generatedMoves |= (*captureCB[piece])(pBoard, origin, color);
	}
	return generatedMoves;
}

// attacks ON a particular color (not BY the color)
BitBoard generateAllAttacks(Board *pBoard, int color) {
	BitBoard attacks = 0;
	BitBoard currentPieces = 0;
	int piece, endpiece;

	if (color == BLACK) {
		color = WHITE;
		piece = WP;
		endpiece = WQ;
	} else {
		color = BLACK;
		piece = BP;
		endpiece = BQ;
	}

	for (; piece <= endpiece; piece++) {
		if (!isPiece(piece)) continue;
		currentPieces = pBoard->position.pieceBB[piece];
		attacks |= generatePieceAttacks(pBoard, color, currentPieces, piece);
	}
	return attacks;
}

BitBoard attacks(Board *pBoard, int attackeeIndex, int side) {
	return BITSET(attackeeIndex) & generateAllAttacks(pBoard, side);
}

bool checks(Board *pBoard, int side) {
	return attacks(pBoard, pBoard->position.kings[side], side) != 0;
}


//===============UNFINISHED===================

void generateCheck(Board *pBoard, MoveSet *pMoves) {
	
	return;
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
