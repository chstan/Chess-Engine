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
			if((movedPiece(m) == WHITE_PAWN || movedPiece(m) == BLACK_PAWN) && (to(m) - from(m) == 16 || from(m) - to(m) == 16)) {
				updateEnPassantSquare(pBoard, ((to(m) + from(m)) / 2));
			} else {
				updateEnPassantSquare(pBoard, INVALID_SQUARE);
			}
		} else {
			printf("ENPASSANT\n");
			setPieceAt(pBoard, to(m), movedPiece(m), 0);
			enPassant(pBoard, color(movedPiece(m)));
		}
		
		if(promote(m)) {
			printf("PROMOTE\n");
			doPromote(pBoard, to(m), promote(m));
			removeMaterial(pBoard, movedPiece(m));
			addMaterial(pBoard, promote(m));
		}
		removeMaterial(pBoard, capturedPiece(m));
	}

	pBoard->info.nextMove ^= 1; // toggle the person to play
}

void unmakeMove(Board *pBoard, Move m) {
	if(whiteCastle(m) | blackCastle(m)) {
		// CASTLING
		if(whiteCastle(m)) {
			unCastle(pBoard, to(m), WHITE);
		} else {
			unCastle(pBoard, to(m), BLACK);
		}
	} else {
		if(promote(m)) {
			unPromote(pBoard, to(m));
			removeMaterial(pBoard, promote(m));
			addMaterial(pBoard, movedPiece(m));
		}
		
		setEmptyAt(pBoard, to(m), movedPiece(m));
		if(!(whiteEnPassant(m) | blackEnPassant(m))) {
			if(capturedPiece(m)) {
				setPieceAt(pBoard, to(m), capturedPiece(m), 0);
			} 
				setPieceAt(pBoard, from(m), movedPiece(m), 0);
		} else {
			// EN PASSANT
			// NEED TO UNDO THE EN PASSANT!!! NOT DONE YET
			setPieceAt(pBoard, from(m), movedPiece(m), 0);
		}
		addMaterial(pBoard, capturedPiece(m));
	}
	pBoard->info.nextMove ^= 1; // toggle the person to play
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
}
