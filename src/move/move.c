#include <stdio.h>
#include <assert.h>

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
		// CASTLING
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
			// EN PASSANT
			setPieceAt(pBoard, to(m), movedPiece(m), 0);
			enPassant(pBoard, color(movedPiece(m)));
		}
		
		if(promote(m)) {
			// PROMOTION
			doPromote(pBoard, to(m), promote(m));
			removeMaterial(pBoard, movedPiece(m));
			addMaterial(pBoard, promote(m));
		}
		removeMaterial(pBoard, capturedPiece(m));
	}
	advanceState(pBoard, m);
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
	rewindState(pBoard, m);
}

void advanceState(Board *pBoard, Move m) {
	int currentMove = pBoard->info.currentMove;
	
	// update according to the fifty move rule
	if(capturedPiece(m) || movedPiece(m) % 8 == WHITE_PAWN) {
		pBoard->info.state[currentMove+1].staleMoves = 0;
	} else {
		pBoard->info.state[currentMove+1].staleMoves = pBoard->info.state[currentMove].staleMoves;
	}
	
	// en passant
	if(movedPiece(m) % 8 == WHITE_PAWN) {
		if((from(m) - to(m) == 16 || to(m) - from(m) == 16)) {
			pBoard->info.state[currentMove+1].enPassantSquare = (to(m) + from(m)) / 2;
		}
	} else {
		pBoard->info.state[currentMove+1].enPassantSquare = INVALID_SQUARE;
	}
	
	// castling
	// white
	if(movedPiece(m) == WHITE_ROOK) {
		if(from(m) == A1) {
			pBoard->info.state[currentMove+1].castleWhite = (pBoard->info.state[currentMove].castleWhite & ~CAN_CASTLE_OOO);
		} else if(from(m) == H1) {
			pBoard->info.state[currentMove+1].castleWhite = (pBoard->info.state[currentMove].castleWhite & ~CAN_CASTLE_OO);
		}
	} else if(movedPiece(m) == WHITE_KING && from(m) == E1) {
		pBoard->info.state[currentMove+1].castleWhite = CANNOT_CASTLE;
	}
	// black
	if(movedPiece(m) == BLACK_ROOK) {
		if(from(m) == A8) {
			pBoard->info.state[currentMove+1].castleBlack = (pBoard->info.state[currentMove].castleBlack & ~CAN_CASTLE_OOO);
		} else if(from(m) == H8) {
			pBoard->info.state[currentMove+1].castleBlack = (pBoard->info.state[currentMove].castleBlack & ~CAN_CASTLE_OO);
		}
	} else if(movedPiece(m) == WHITE_ROOK && from(m) == A1) {
		pBoard->info.state[currentMove+1].castleBlack = CANNOT_CASTLE;
	}
	
	// update the move index and switch colors to play
	pBoard->info.currentMove++;
	assert(pBoard->info.currentMove < MAX_MOVES_PER_GAME);
	pBoard->info.toPlay ^= 1;
}

void rewindState(Board *pBoard, Move m) {
	// we don't need to do anything else, the board simply forgets about what
	// happened in the future board states and we roll back to the last
	pBoard->info.currentMove--;
	assert(pBoard->info.currentMove >= 0);
	pBoard->info.toPlay ^= 1;
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
