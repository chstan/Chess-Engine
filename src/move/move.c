#include <stdio.h>

#include "move.h"
#include "../board/board.h"
#include "../defines.h"
#include "../extglobals.h"

/* As a word of note:
 *   These functions assume that the move
 * is valid for the board and that the movement
 * obeys all piece laws. The rest is up to
 * the move generator itself. Furthermore
 * it assumes that the data contained in
 * m is coherent.
 */

void makeMove(Board *pBoard, Move m) {
	if(whiteCastle(m) | blackCastle(m)) {
		printf("CASTLING\n");
		if(whiteCastle(m)) {
			castle(pBoard, to(m), WHITE);
		} else {
			castle(pBoard, to(m), BLACK);
		}
	} else {
		setEmptyAt(pBoard, from(m), movedPiece(m));
		
		if(!(whiteEnPassant(m) | blackEnPassant(m))) {
			setPieceAt(pBoard, to(m), movedPiece(m), capturedPiece(m));
		} else {
			printf("ENPASSANT\n");
			setPieceAt(pBoard, to(m), movedPiece(m), 0);
			enPassant(pBoard);
		}
		
		if(promote(m)) {
			printf("PROMOTE\n");
			doPromote(pBoard, to(m), promote(m));
			removeMaterial(pBoard, movedPiece(m));
			addMaterial(pBoard, promote(m));
		}
		removeMaterial(pBoard, capturedPiece(m));
	}
	pBoard->info.nextMove = (pBoard->info.nextMove + 1)%2;
	return;
}

void unmakeMove(Board *pBoard, Move m) {
	// UNFINISHED
	return;
}

void printMove(Move m) {
	static const char *lineA =
	"\tH G F E D C B A #PROMO# #CAPTD#\n"
	"\t- - - - - - - - ------- -------\n";
	static const char *lineB =
	"\t#MOVED# #FROM BITS# # TO BITS #\n"
	"\t------- ----------- -----------\n";
	static const char *footer=
	"A:    WHITE CASTLING\n"
	"B:    BLACK CASTLING\n"
	"C:    WHITE ENPASSANT\n"
	"D:    BLACK ENPASSANT\n"
	"E-H:  UNUSED FLAGS\n";
	printf("%s\t", lineA);
	for(int i = 31; i >= 16; i--) {
		printf("%d ", (m & BITSET[i]) ? 1 : 0);
	}
	printf("\n%s\t", lineB);
	for(int i = 15; i >= 0; i--) {
		printf("%d ", (m & BITSET[i]) ? 1 : 0);
	}
	printf("\n%s", footer);
	return;
}
