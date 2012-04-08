#include "protos.h"
#include "board.h"
#include "move.h"
#include "defines.h"
#include "extglobals.h"


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
			setPieceAt(pBoard, to(m), movedPiece(m), 0);
			enPassant(pBoard);
		}
		
		if(promote(m)) {
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