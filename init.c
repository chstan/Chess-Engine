#include "defines.h"
#include "protos.h"
#include "extglobals.h"
#include "board.h"
#include <stdlib.h>
#include <stdio.h>

void init(void) {
	dataInit();
	moveTableInit();
	boardInit();
	return;
}

void moveTableInit(void) {
	int rank, file, index;
	int lRank, hRank, lFile, hFile;
	BitMap bits;
	// i will be rank and j will be file
	for(rank = 1; rank <= 8; rank++) {
		for(file = 1; file <= 8; file++) {
			index = BOARDINDEX[rank][file];
			
			//--------PAWN ONE SQUARE----------
			if(rank < 7){
				pawnMoveWhite[index] = BITSET[index + 8];
			} else {
				pawnMoveWhite[index] = emptyBoard;
			}
			
			if(rank > 2) {
				pawnMoveBlack[index] = BITSET[index - 8];
			} else {
				pawnMoveBlack[index] = emptyBoard;
			}
			
			//--------PAWN TWO SQUARES---------
			if(rank == 2) {
				pawnDoubleWhite[index] = BITSET[index + 16];
			} else {
				pawnDoubleWhite[index] = emptyBoard;
			}
			if(rank == 7) {
				pawnDoubleBlack[index] = BITSET[index - 16];
			} else {
				pawnDoubleBlack[index] = emptyBoard;
			}
			
			//----------PAWN CAPTURES----------
			if(8 > file && file > 1) {
				if (rank <= 6) pawnCaptureWhite[index] = BITSET[index + 9] | BITSET[index + 7];
				if (rank >= 3) pawnCaptureBlack[index] = BITSET[index - 9] | BITSET[index - 7];
			} else {
				if(file == 1) {
					pawnCaptureWhite[index] = emptyBoard;
					pawnCaptureBlack[index] = emptyBoard;
					if (rank <= 6) pawnCaptureWhite[index] = BITSET[index + 9];
					if (rank >= 3) pawnCaptureBlack[index] = BITSET[index - 7];
				} else {
					pawnCaptureWhite[index] = emptyBoard;
					pawnCaptureBlack[index] = emptyBoard;
					if (rank <= 6) pawnCaptureWhite[index] = BITSET[index + 7];
					if (rank >= 3) pawnCaptureBlack[index] = BITSET[index - 9];
				}
			}
			
			//-----PAWN CAPTURE PROMOTIONS-----
			if(8 > file && file > 1) {
				if (rank == 7) pawnPromotionCaptureWhite[index] = BITSET[index + 9] | BITSET[index + 7];
				if (rank == 2) pawnPromotionCaptureBlack[index] = BITSET[index - 9] | BITSET[index - 7];
			} else {
				if(file == 1) {
					pawnPromotionCaptureWhite[index] = emptyBoard;
					pawnPromotionCaptureBlack[index] = emptyBoard;
					if (rank == 7) pawnPromotionCaptureWhite[index] = BITSET[index + 9];
					if (rank == 2) pawnPromotionCaptureBlack[index] = BITSET[index - 7];
				} else {
					pawnPromotionCaptureWhite[index] = emptyBoard;
					pawnPromotionCaptureBlack[index] = emptyBoard;
					if (rank == 7) pawnPromotionCaptureWhite[index] = BITSET[index + 7];
					if (rank == 2) pawnPromotionCaptureBlack[index] = BITSET[index - 9];
				}
			}
			
			//---------PAWN PROMOTIONS---------
			if(rank == 7) {
				pawnPromotionWhite[index] = BITSET[index + 8];
			} else {
				pawnPromotionWhite[index] = emptyBoard;
			}
			if(rank == 2) {
				pawnPromotionBlack[index] = BITSET[index - 8];
			} else {
				pawnPromotionBlack[index] = emptyBoard;	
			}
			
			//--------KING    MOVEMENTS--------
			lRank = (rank - 1 >= 1) ? rank - 1 : 1;
			hRank = (rank + 1 <= 8) ? rank + 1 : 8;
			lFile = (file - 1 >= 1) ? file - 1 : 1;
			hFile = (file + 1 <= 8) ? file + 1 : 8;
			bits = 0;
			int iF;
			int iR;
			for(iR = lRank; iR <= hRank; iR++) {
				for(iF = lFile; iF <= hFile; iF++) {
					bits |= BITSET[sq(iR, iF)];
				}
			}
			kingMove[index] = bits & ~BITSET[index];
			
			//--------KNIGHT  MOVEMENTS--------
			bits = 0;
			if(rank + 2 <= 8 && file + 1 <= 8) {
				bits |= BITSET[sq(rank+2, file+1)];
			}
			if(rank + 2 <= 8 && file - 1 >= 1) {
				bits |= BITSET[sq(rank+2, file-1)];
			}
			if(rank + 1 <= 8 && file + 2 <= 8) {
				bits |= BITSET[sq(rank+1, file+2)];
			}
			if(rank + 1 <= 8 && file - 2 >= 1) {
				bits |= BITSET[sq(rank+1, file-2)];
			}
			if(rank - 1 >= 1 && file + 2 <= 8) {
				bits |= BITSET[sq(rank-1, file+2)];
			}
			if(rank - 1 >= 1 && file - 2 >= 1) {
				bits |= BITSET[sq(rank-1, file-2)];
			}
			if(rank - 2 >= 1 && file + 1 <= 8) {
				bits |= BITSET[sq(rank-2, file+1)];
			}
			if(rank - 2 >= 1 && file - 1 >= 1) {
				bits |= BITSET[sq(rank-2, file-1)];
			}
			knightMove[index] = bits;
			
			//--------DIAGON  MOVEMENTS--------
			for(lFile = file+1, lRank = rank+1, bits = 0; lFile <= 8 && lRank <= 8; lFile++, lRank++) {
				bits |= BITSET[sq(lFile, lRank)];
			}
			rightUpMove[index] = bits;
			
			for(lFile = file+1, lRank = rank-1, bits = 0; lFile <= 8 && lRank >= 1; lFile++, lRank--) {
				bits |= BITSET[sq(lFile, lRank)];
			}
			leftUpMove[index] = bits;
			
			for(lFile = file-1, lRank = rank+1, bits = 0; lFile >= 1 && lRank <= 8; lFile--, lRank++) {
				bits |= BITSET[sq(lFile, lRank)];
			}
			rightDownMove[index] = bits;
			
			for(lFile = file-1, lRank = rank-1, bits = 0; lFile >= 1 && lRank >= 1; lFile--, lRank--) {
				bits |= BITSET[sq(lFile, lRank)];
			}
			leftDownMove[index] = bits;
			
			//--------ORTHOG  MOVEMENTS--------
			for(lFile = file+1, bits = 0; lFile <= 8; lFile++) {
				bits |= BITSET[sq(lFile, rank)];
			}
			upMove[index] = bits;
			
			for(lFile = file-1, bits = 0; lFile >= 1; lFile--) {
				bits |= BITSET[sq(lFile, rank)];
			}
			downMove[index] = bits;
			
			for(lRank = rank+1, bits = 0; lRank <= 8; lRank++) {
				bits |= BITSET[sq(file, lRank)];
			}
			rightMove[index] = bits;
			
			for(lRank = rank-1, bits = 0; lRank >= 1; lRank--) {
				bits |= BITSET[sq(file, lRank)];
			}
			leftMove[index] = bits;
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
			BOARDINDEX[rank][file] = sq(rank, file);
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