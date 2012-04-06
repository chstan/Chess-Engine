#include "defines.h"
#include "protos.h"
#include "extglobals.h"
#include "board.h"
#include <stdlib.h>

void init(void) {
	dataInit();
	moveTableInit();
	boardInit();
	return;
}

void moveTableInit(void) {
	int i, j, index;
	// i will be rank and j will be file
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			index = BOARDINDEX[i+1][j+1];
			
			//---------PAWN ONE SQUARE---------
			if(i != 7){
				pawnMoveWhite[index] = SETBIT[index + 8];
			} else {
				pawnMoveWhite[index] = emptyBoard;
			}
			if(i != 0) {
				pawnMoveBlack[index] = SETBIT[index - 8];
			} else {
				pawnMoveBlack[index] = emptyBoard;
			}
			
			//---------PAWN TWO SQUARES--------
			if(i == 2) {
				pawnDoubleWhite[index] = SETBIT[index + 16];
			} else {
				pawnDoubleWhite[index] = emptyBoard;
			}
			if(i == 7) {
				pawnDoubleBlack[index] = SETBIT[index - 16];
			} else {
				pawnDoubleBlack[index] = emptyboard;
			}
			
			//----------PAWN CAPTURES----------
			
			
			//---------PAWN PROMOTIONS---------
			//--------KING    MOVEMENTS--------
			//--------KNIGHT  MOVEMENTS--------
			//--------DIAGON  MOVEMENTS--------
			//--------ORTHOG  MOVEMENTS--------
		}
	}
	return;
}

void dataInit(void) {
	int i, rank, file;
	
	BITSET[0] = 0x1;
	for(i = 1; i < 64; i++) {
		BITSET[i] = BITSET[i-1] << 1;
	}
	
	for(rank = 0; rank < 9; rank++) {
		for(file = 0; file < 9; file++) {
			BOARDINDEX[file][rank] = sq(rank, file);
		}
	}
	
	for(i = 0; i < 256; i++) {
    GS1B[i] = (
        (i > 127) ? 7 :
        (i >  63) ? 6 :
        (i >  31) ? 5 :
        (i >  15) ? 4 :
        (i >   7) ? 3 :
        (i >   3) ? 2 :
        (i >   1) ? 1 : 0 );
	}
	return;
}

void boardInit(void) {
	pBoard = malloc(sizeof(Board));
	resetBoard(pBoard);
	return;
}