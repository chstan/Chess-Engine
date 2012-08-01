#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "../bit.h"
#include "../defines.h"
#include "../extglobals.h"
#include "../move/move.h"

void resetBoard(Board *pBoard) {
	int i;
	for(i = 0; i < 64; i++) pBoard->position.square[i] = EMPTY;
	pBoard->position.square[A1] = WHITE_ROOK;
	pBoard->position.square[B1] = WHITE_KNIGHT;
	pBoard->position.square[C1] = WHITE_BISHOP;
	pBoard->position.square[D1] = WHITE_QUEEN;
	pBoard->position.square[E1] = WHITE_KING;
	pBoard->position.square[F1] = WHITE_BISHOP;
	pBoard->position.square[G1] = WHITE_KNIGHT;
	pBoard->position.square[H1] = WHITE_ROOK;
	pBoard->position.square[A2] = WHITE_PAWN;
	pBoard->position.square[B2] = WHITE_PAWN;
	pBoard->position.square[C2] = WHITE_PAWN;
	pBoard->position.square[D2] = WHITE_PAWN;
	pBoard->position.square[E2] = WHITE_PAWN;
	pBoard->position.square[F2] = WHITE_PAWN;
	pBoard->position.square[G2] = WHITE_PAWN;
	pBoard->position.square[H2] = WHITE_PAWN;
	
	pBoard->position.square[A8] = BLACK_ROOK;
	pBoard->position.square[B8] = BLACK_KNIGHT;
	pBoard->position.square[C8] = BLACK_BISHOP;
	pBoard->position.square[D8] = BLACK_QUEEN;
	pBoard->position.square[E8] = BLACK_KING;
	pBoard->position.square[F8] = BLACK_BISHOP;
	pBoard->position.square[G8] = BLACK_KNIGHT;
	pBoard->position.square[H8] = BLACK_ROOK;
	pBoard->position.square[A7] = BLACK_PAWN;
	pBoard->position.square[B7] = BLACK_PAWN;
	pBoard->position.square[C7] = BLACK_PAWN;
	pBoard->position.square[D7] = BLACK_PAWN;
	pBoard->position.square[E7] = BLACK_PAWN;
	pBoard->position.square[F7] = BLACK_PAWN;
	pBoard->position.square[G7] = BLACK_PAWN;
	pBoard->position.square[H7] = BLACK_PAWN;
	
	pBoard->info.displayRotated = false;
	
	initBoardFromSquares(pBoard, WHITE, 0, CAN_CASTLE_OO + CAN_CASTLE_OOO + CAN_CASTLE,
		CAN_CASTLE_OO + CAN_CASTLE_OOO + CAN_CASTLE, 0);
	return;
}

void initBoardFromSquares(Board* pBoard, unsigned char nextMove, int staleMoves, int castleW, 
int castleB, int enPassantSquare) {
	
	resetBB(pBoard);
	updateBBFromSquares(pBoard);
	updateAggregateBB(pBoard);
	updateMaterialFromBB(pBoard);
	updateKingsFromBB(pBoard);
	updatePieceCountsFromBB(pBoard);
	
	pBoard->info.nextMove = nextMove;
	pBoard->info.castleWhite = castleW;
	pBoard->info.castleBlack = castleB;
	pBoard->info.enPassantSquare = enPassantSquare;
	pBoard->info.staleMoves = staleMoves;
	return;
}

void displayBoard(Board *pBoard) {
	const char *header  = "   A     B     C     D     E     F     G     H   ";
	const char *reverse = "   H     G     F     E     D     C     B     A   ";
	const char *divider = "+-----+-----+-----+-----+-----+-----+-----+-----+";
	
	printf("\n\t%s\n", pBoard->info.nextMove ? reverse : header);
	
	int i, j;
	for(i = 0; i < 8; i++) {
		printf("\t%s\n\t", divider);
		for(j = 0; j < 8; j++) {
			switch(pBoard->info.nextMove) {
				case W:
					printf("| %s ", PIECENAMES[pBoard->position.square[sq(8-i, j+1)]]);	
				break;
				case B:
					printf("| %s ", PIECENAMES[pBoard->position.square[sq(i+1, 8-j)]]);
				break;
			}
		}
		printf("|  %d\n", pBoard->info.nextMove ? i+1 : 8-i);
	}
	printf("\t%s\n\n", divider);
	printf("Material counts:\n\tWhite: %d\n\tBlack: %d\n", pBoard->info.whiteMaterial, pBoard->info.blackMaterial);
	if(!(pBoard->info.castleWhite & CAN_CASTLE)) printf("White cannot castle.\n");
	else {
		printf("White can castle");
		if(pBoard->info.castleWhite == CAN_CASTLE_OOO)
			printf(" queenside");
		if(pBoard->info.castleWhite == CAN_CASTLE_OO)
			printf(" kingside");
		printf(".\n");
	}
	if(!(pBoard->info.castleBlack & CAN_CASTLE)) printf("Black cannot castle.\n");
	else {
		printf("Black can castle");
		if(pBoard->info.castleBlack == CAN_CASTLE_OOO)
			printf(" queenside");
		if(pBoard->info.castleBlack == CAN_CASTLE_OO)
			printf(" kingside");
		printf(".\n");
	}
	return;
}

void rotateBoard(Board *pBoard) {
	if(pBoard->info.displayRotated) {
		pBoard->info.displayRotated = false;
	} else {
		pBoard->info.displayRotated = true;
	}
	return;
}

void debugBoard(Board *pBoard) {
	// we first check if the board is in a consistent state, and report to the user
	// certain problems cannot be found at runtime however, and require prior knowledge of the
	// state of the board, for instance in the case of castling.
	bool boardConsistent = true;
	
	// array of squares
	for(int i = 0; i < 64; i++) {
		switch(pBoard->position.square[i]) {
			case E:
				// nothing to handle here, we don't track empty squares
			break;
			case WP:
				if(!(BITSET[i] & pBoard->position.pieceBB[WHITE_PAWN])) {
					boardConsistent = false;
					printf("The array of squares reports a white pawn at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;  
			case WK:
				if(!(BITSET[i] & pBoard->position.pieceBB[WHITE_KING])) {
					boardConsistent = false;
					printf("The array of squares reports a white king at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case WN:
				if(!(BITSET[i] & pBoard->position.pieceBB[WHITE_KNIGHT])) {
					boardConsistent = false;
					printf("The array of squares reports a white knight at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case WB:
				if(!(BITSET[i] & pBoard->position.pieceBB[WHITE_BISHOP])) {
					boardConsistent = false;
					printf("The array of squares reports a white bishop at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case WR:
				if(!(BITSET[i] & pBoard->position.pieceBB[WHITE_ROOK])) {
					boardConsistent = false;
					printf("The array of squares reports a white rook at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case WQ:
				if(!(BITSET[i] & pBoard->position.pieceBB[WHITE_QUEEN])) {
					boardConsistent = false;
					printf("The array of squares reports a white queen at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case BP:
				if(!(BITSET[i] & pBoard->position.pieceBB[BLACK_PAWN])) {
					boardConsistent = false;
					printf("The array of squares reports a black pawn at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case BK:
				if(!(BITSET[i] & pBoard->position.pieceBB[BLACK_KING])) {
					boardConsistent = false;
					printf("The array of squares reports a black king at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case BN:
				if(!(BITSET[i] & pBoard->position.pieceBB[BLACK_KNIGHT])) {
					boardConsistent = false;
					printf("The array of squares reports a black knight at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case BB:
				if(!(BITSET[i] & pBoard->position.pieceBB[BLACK_BISHOP])) {
					boardConsistent = false;
					printf("The array of squares reports a black bishop at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
			case BR:
				if(!(BITSET[i] & pBoard->position.pieceBB[BLACK_ROOK])) {
					boardConsistent = false;
					printf("The array of squares reports a black rook at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break; 
			case BQ:
				if(!(BITSET[i] & pBoard->position.pieceBB[BLACK_QUEEN])) {
					boardConsistent = false;
					printf("The array of squares reports a black queen at %s, but the bitboard is empty.\n", SQUARENAME[i]);
				}
			break;
		}
	}
	
	// array of kings
	if(!(BITSET[pBoard->position.kings[BLACK]] & pBoard->position.pieceBB[BLACK_KING])) {
		boardConsistent = false;
		printf("The array of kings reports a black king at %s, but the bitboard does not.\n", SQUARENAME[pBoard->position.kings[BLACK]]);
	}
	if(!(BITSET[pBoard->position.kings[WHITE]] & pBoard->position.pieceBB[WHITE_KING])) {
		boardConsistent = false;
		printf("The array of kings reports a white king at %s, but the bitboard does not.\n", SQUARENAME[pBoard->position.kings[WHITE]]);
	}
	
	// array of pieces
	// no real check needed here at the moment, though one could be added for material consistency
	
	// bitboards
	// white pawn
	
	// white king
	// white queen
	// white bishop
	// white knight
	// white rook
	// black pawn
	// black king
	// black queen
	// black bishop
	// black knight
	// black rook
	
	// rudimentary castling checks
	switch(pBoard->info.castleWhite) {
		case CAN_CASTLE:
			if(pBoard->position.square[A1] != WHITE_ROOK || pBoard->position.square[A8] != WHITE_ROOK || pBoard->position.square[E1] != WHITE_KING) {
				boardConsistent = false;
				printf("The board info incorrectly reports that white can castle queenside or kingside.\n");
			}
		break;
		case CAN_CASTLE_OO:
			if(pBoard->position.square[E1] != WHITE_KING || pBoard->position.square[A8] != WHITE_ROOK) {
				boardConsistent = false;
				printf("The board info incorrectly reports that white can castle kingside.");
			}
		break;
		case CAN_CASTLE_OOO:
			if(pBoard->position.square[E1] != WHITE_KING || pBoard->position.square[A1] != WHITE_ROOK) {
				boardConsistent = false;
				printf("The board info incorrectly reports that white can castle queenside.");
			}
		break;
	}
	
	switch(pBoard->info.castleBlack) {
		case CAN_CASTLE:
			if(pBoard->position.square[A1] != BLACK_ROOK || pBoard->position.square[A8] != BLACK_ROOK || pBoard->position.square[E1] != BLACK_KING) {
				boardConsistent = false;
				printf("The board info incorrectly reports that black can castle queenside or kingside.\n");
			}
		break;
		case CAN_CASTLE_OO:
			if(pBoard->position.square[E1] != BLACK_KING || pBoard->position.square[A8] != BLACK_ROOK) {
				boardConsistent = false;
				printf("The board info incorrectly reports that black can castle kingside.");
			}
		break;
		case CAN_CASTLE_OOO:
			if(pBoard->position.square[E1] != BLACK_KING || pBoard->position.square[A1] != BLACK_ROOK) {
				boardConsistent = false;
				printf("The board info incorrectly reports that black can castle queenside.");
			}
		break;
	}
	
	// if it is not, we will print out a diagnostic
	if(!boardConsistent) {
		displayBoard(pBoard);
		// other info would be nice too, but this is fine for the moment
	}
}

//---------------------MAKE FUNCTIONS------------------------
void setEmptyAt(Board *pBoard, UCHAR index, UCHAR lastOccupant) {
	//DOES NOT UPDATE ANY MATERIAL COUNTS
	switch(lastOccupant) {
		case WP:
			pBoard->position.pieceBB[WHITE_PAWN] &= ~BITSET[index];
			pBoard->position.whiteOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case BP:
			pBoard->position.pieceBB[BLACK_PAWN] &= ~BITSET[index];
			pBoard->position.blackOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case WK:
			pBoard->position.pieceBB[WHITE_KING] &= ~BITSET[index];
			pBoard->position.whiteOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
			pBoard->position.kings[WHITE] = INVALID_SQUARE;
		break;
		case BK:
			pBoard->position.pieceBB[BLACK_KING] &= ~BITSET[index];
			pBoard->position.blackOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
			pBoard->position.kings[WHITE] = INVALID_SQUARE;
		break;
		case WQ:
			pBoard->position.pieceBB[WHITE_QUEEN] &= ~BITSET[index];
			pBoard->position.whiteOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case BQ:
			pBoard->position.pieceBB[BLACK_QUEEN] &= ~BITSET[index];
			pBoard->position.blackOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case WB:
			pBoard->position.pieceBB[WHITE_BISHOP] &= ~BITSET[index];
			pBoard->position.whiteOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case BB:
			pBoard->position.pieceBB[BLACK_BISHOP] &= ~BITSET[index];
			pBoard->position.blackOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case WN:
			pBoard->position.pieceBB[WHITE_KNIGHT] &= ~BITSET[index];
			pBoard->position.whiteOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case BN:
			pBoard->position.pieceBB[BLACK_KNIGHT] &= ~BITSET[index];
			pBoard->position.blackOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case WR:
			pBoard->position.pieceBB[WHITE_ROOK] &= ~BITSET[index];
			pBoard->position.whiteOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case BR:
			pBoard->position.pieceBB[BLACK_ROOK] &= ~BITSET[index];
			pBoard->position.blackOccupied &= ~BITSET[index];
			pBoard->position.occupied &= ~BITSET[index];
			pBoard->position.square[index] = EMPTY;
		break;
		case E:
		break;
	}
	return;
}

void setPieceAt(Board *pBoard, UCHAR index, UCHAR movedPiece, UCHAR capturedPiece) {
	if(capturedPiece) {
		setEmptyAt(pBoard, index, capturedPiece);
	}
	switch(movedPiece) {
		case WP:
			pBoard->position.pieceBB[WHITE_PAWN] |= BITSET[index];
			pBoard->position.whiteOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case BP:
			pBoard->position.pieceBB[BLACK_PAWN] |= BITSET[index];
			pBoard->position.blackOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case WK:
			pBoard->position.pieceBB[WHITE_KING] |= BITSET[index];
			pBoard->position.whiteOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
			pBoard->position.kings[WHITE] = index;
		break;
		case BK:
			pBoard->position.pieceBB[BLACK_KING] |= BITSET[index];
			pBoard->position.blackOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
			pBoard->position.kings[WHITE] = index;
		break;
		case WQ:
			pBoard->position.pieceBB[WHITE_QUEEN] |= BITSET[index];
			pBoard->position.whiteOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case BQ:
			pBoard->position.pieceBB[BLACK_QUEEN] |= BITSET[index];
			pBoard->position.blackOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case WB:
			pBoard->position.pieceBB[WHITE_BISHOP] |= BITSET[index];
			pBoard->position.whiteOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case BB:
			pBoard->position.pieceBB[BLACK_BISHOP] |= BITSET[index];
			pBoard->position.blackOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case WN:
			pBoard->position.pieceBB[WHITE_KNIGHT] |= BITSET[index];
			pBoard->position.whiteOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case BN:
			pBoard->position.pieceBB[BLACK_KNIGHT] |= BITSET[index];
			pBoard->position.blackOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case WR:
			pBoard->position.pieceBB[WHITE_ROOK] |= BITSET[index];
			pBoard->position.whiteOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case BR:
			pBoard->position.pieceBB[BLACK_ROOK] |= BITSET[index];
			pBoard->position.blackOccupied |= BITSET[index];
			pBoard->position.occupied |= BITSET[index];
			pBoard->position.square[index] = movedPiece;
		break;
		case E:
		break;
	}
	return;
}

void enPassant(Board *pBoard) {
	int index = pBoard->info.enPassantSquare;
	int piece = pBoard->position.square[index];
	setEmptyAt(pBoard, index, piece);
	return;
}

void castle(Board *pBoard, UCHAR index, UCHAR whichKing) {
	switch(whichKing) {
		case W:
			assert(index == C1 || index == G1);
			if(index == C1) {
				setEmptyAt(pBoard, E1, WHITE_KING);
				setPieceAt(pBoard, C1, WHITE_KING, 0);
				setEmptyAt(pBoard, A1, WHITE_ROOK);
				setPieceAt(pBoard, D1, WHITE_ROOK, 0);
				pBoard->info.castleWhite &= ~CAN_CASTLE_OOO;
			} else {
				setEmptyAt(pBoard, E1, WHITE_KING);
				setPieceAt(pBoard, G1, WHITE_KING, 0);
				setEmptyAt(pBoard, H1, WHITE_ROOK);
				setPieceAt(pBoard, F1, WHITE_ROOK, 0);
				pBoard->info.castleWhite &= ~CAN_CASTLE_OO;
			}
			pBoard->info.castleWhite &= ~CAN_CASTLE;
		break;
		case B:
			assert(index == C8 || index == G8);
			if(index == C8) {
				setEmptyAt(pBoard, E8, BLACK_KING);
				setPieceAt(pBoard, C8, BLACK_KING, 0);
				setEmptyAt(pBoard, A8, BLACK_ROOK);
				setPieceAt(pBoard, D8, BLACK_ROOK, 0);
				pBoard->info.castleBlack &= ~CAN_CASTLE_OOO;
			} else {                 
				setEmptyAt(pBoard, E8, BLACK_KING);
				setPieceAt(pBoard, G8, BLACK_KING, 0);
				setEmptyAt(pBoard, H8, BLACK_ROOK);
				setPieceAt(pBoard, F8, BLACK_ROOK, 0);
				pBoard->info.castleBlack &= ~CAN_CASTLE_OO;
			}
			pBoard->info.castleBlack &= ~CAN_CASTLE;
		break;
	}
	return;
}

void unCastle(Board *pBoard, UCHAR index, UCHAR whichKing) {
	switch(whichKing) {
		case W:
			assert(index == C1 || index == G1);
			if(index == C1) {
				setEmptyAt(pBoard, C1, WHITE_KING);
				setPieceAt(pBoard, E1, WHITE_KING, 0);
				setEmptyAt(pBoard, D1, WHITE_ROOK);
				setPieceAt(pBoard, A1, WHITE_ROOK, 0);
				pBoard->info.castleWhite |= CAN_CASTLE_OOO;
			} else {
				setEmptyAt(pBoard, G1, WHITE_KING);
				setPieceAt(pBoard, E1, WHITE_KING, 0);
				setEmptyAt(pBoard, F1, WHITE_ROOK);
				setPieceAt(pBoard, H1, WHITE_ROOK, 0);
				pBoard->info.castleWhite |= CAN_CASTLE_OO;
			}
			pBoard->info.castleWhite |= CAN_CASTLE;
		break;
		case B:
			assert(index == C8 || index == G8);
			if(index == C8) {
				setEmptyAt(pBoard, C8, BLACK_KING);
				setPieceAt(pBoard, E8, BLACK_KING, 0);
				setEmptyAt(pBoard, D8, BLACK_ROOK);
				setPieceAt(pBoard, A8, BLACK_ROOK, 0);
				pBoard->info.castleBlack |= CAN_CASTLE_OOO;
			} else {                 
				setEmptyAt(pBoard, G8, BLACK_KING);
				setPieceAt(pBoard, E8, BLACK_KING, 0);
				setEmptyAt(pBoard, F8, BLACK_ROOK);
				setPieceAt(pBoard, H8, BLACK_ROOK, 0);	
				pBoard->info.castleBlack |= CAN_CASTLE_OO;
			}
			pBoard->info.castleBlack |= CAN_CASTLE;
		break;
	}
	return;
}

void doPromote(Board *pBoard, UCHAR index, UCHAR whichPromote) {
	switch(color(whichPromote)) {
		case W:
			setEmptyAt(pBoard, index, WHITE_PAWN);
			setPieceAt(pBoard, index, whichPromote, 0);
		break;
		case B:
			setEmptyAt(pBoard, index, BLACK_PAWN);
			setPieceAt(pBoard, index, whichPromote, 0);
		break;
	}
	return;
}

void unPromote(Board *pBoard, UCHAR index) {
	switch(color(pBoard->position.square[index])) {
		case W:
			setEmptyAt(pBoard, index, pBoard->position.square[index]);
			setPieceAt(pBoard, index, WHITE_PAWN, 0);
		break;
		case B:
			setEmptyAt(pBoard, index, pBoard->position.square[index]);
			setPieceAt(pBoard, index, BLACK_PAWN, 0);		
		break;
	}
	return;
}

void addMaterial(Board *pBoard, UCHAR piece) {
	switch(piece) {
		case WP:
			pBoard->info.whiteMaterial += PAWN_VALUE;
			pBoard->position.pieces[WHITE][PAWN]++;
			pBoard->position.pieces[WHITE][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case BP:
			pBoard->info.blackMaterial += PAWN_VALUE;
			pBoard->position.pieces[BLACK][PAWN]++;
			pBoard->position.pieces[BLACK][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case WK:
			pBoard->info.whiteMaterial += KING_VALUE;
			pBoard->position.pieces[WHITE][KING]++;
			pBoard->position.pieces[WHITE][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case BK:
			pBoard->info.blackMaterial += KING_VALUE;
			pBoard->position.pieces[BLACK][KING]++;
			pBoard->position.pieces[BLACK][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case WQ:
			pBoard->info.whiteMaterial += QUEEN_VALUE;
			pBoard->position.pieces[WHITE][QUEEN]++;
			pBoard->position.pieces[WHITE][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case BQ:
			pBoard->info.blackMaterial += QUEEN_VALUE;
			pBoard->position.pieces[BLACK][QUEEN]++;
			pBoard->position.pieces[BLACK][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case WB:
			pBoard->info.whiteMaterial += BISHOP_VALUE;
			pBoard->position.pieces[WHITE][BISHOP]++;
			pBoard->position.pieces[WHITE][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case BB:
			pBoard->info.blackMaterial += BISHOP_VALUE;
			pBoard->position.pieces[BLACK][BISHOP]++;
			pBoard->position.pieces[BLACK][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case WN:
			pBoard->info.whiteMaterial += KNIGHT_VALUE;
			pBoard->position.pieces[WHITE][KNIGHT]++;
			pBoard->position.pieces[WHITE][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case BN:
			pBoard->info.blackMaterial += KNIGHT_VALUE;
			pBoard->position.pieces[BLACK][KNIGHT]++;
			pBoard->position.pieces[BLACK][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case WR:
			pBoard->info.whiteMaterial += ROOK_VALUE;
			pBoard->position.pieces[WHITE][ROOK]++;
			pBoard->position.pieces[WHITE][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
		case BR:
			pBoard->info.blackMaterial += ROOK_VALUE;
			pBoard->position.pieces[BLACK][ROOK]++;
			pBoard->position.pieces[BLACK][TOTAL]++;
			pBoard->position.totalPieces++;
		break;
	}
	pBoard->info.material = pBoard->info.whiteMaterial - pBoard->info.blackMaterial;
	return;
}

void removeMaterial(Board *pBoard, UCHAR piece) {
	switch(piece) {
		case WP:
			pBoard->info.whiteMaterial -= PAWN_VALUE;
			pBoard->position.pieces[WHITE][PAWN]--;
			pBoard->position.pieces[WHITE][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case BP:
			pBoard->info.blackMaterial -= PAWN_VALUE;
			pBoard->position.pieces[BLACK][PAWN]--;
			pBoard->position.pieces[BLACK][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case WK:
			pBoard->info.whiteMaterial -= KING_VALUE;
			pBoard->position.pieces[WHITE][KING]--;
			pBoard->position.pieces[WHITE][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case BK:
			pBoard->info.blackMaterial -= KING_VALUE;
			pBoard->position.pieces[BLACK][KING]--;
			pBoard->position.pieces[BLACK][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case WQ:
			pBoard->info.whiteMaterial -= QUEEN_VALUE;
			pBoard->position.pieces[WHITE][QUEEN]--;
			pBoard->position.pieces[WHITE][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case BQ:
			pBoard->info.blackMaterial -= QUEEN_VALUE;
			pBoard->position.pieces[BLACK][QUEEN]--;
			pBoard->position.pieces[BLACK][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case WB:
			pBoard->info.whiteMaterial -= BISHOP_VALUE;
			pBoard->position.pieces[WHITE][BISHOP]--;
			pBoard->position.pieces[WHITE][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case BB:
			pBoard->info.blackMaterial -= BISHOP_VALUE;
			pBoard->position.pieces[BLACK][BISHOP]--;
			pBoard->position.pieces[BLACK][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case WN:
			pBoard->info.whiteMaterial -= KNIGHT_VALUE;
			pBoard->position.pieces[WHITE][KNIGHT]--;
			pBoard->position.pieces[WHITE][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case BN:
			pBoard->info.blackMaterial -= KNIGHT_VALUE;
			pBoard->position.pieces[BLACK][KNIGHT]--;
			pBoard->position.pieces[BLACK][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case WR:
			pBoard->info.whiteMaterial -= ROOK_VALUE;
			pBoard->position.pieces[WHITE][ROOK]--;
			pBoard->position.pieces[WHITE][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
		case BR:
			pBoard->info.blackMaterial -= ROOK_VALUE;
			pBoard->position.pieces[BLACK][ROOK]--;
			pBoard->position.pieces[BLACK][TOTAL]--;
			pBoard->position.totalPieces--;
		break;
	}
	pBoard->info.material = pBoard->info.whiteMaterial - pBoard->info.blackMaterial;
	return;
}

//---------------------AUX FUNCTIONS-------------------------
void updatePieceCountsFromBB(Board *pBoard) {
	// Slow. Consider looping through squares
	pBoard->position.pieces[WHITE][PAWN] = countBits(pBoard->position.pieceBB[WHITE_PAWN]);
	pBoard->position.pieces[WHITE][BISHOP] = countBits(pBoard->position.pieceBB[WHITE_BISHOP]);
	pBoard->position.pieces[WHITE][KNIGHT] = countBits(pBoard->position.pieceBB[WHITE_KNIGHT]);
	pBoard->position.pieces[WHITE][ROOK] = countBits(pBoard->position.pieceBB[WHITE_ROOK]);
	pBoard->position.pieces[WHITE][QUEEN] = countBits(pBoard->position.pieceBB[WHITE_QUEEN]);
	pBoard->position.pieces[WHITE][KING] = countBits(pBoard->position.pieceBB[WHITE_KING]);
	pBoard->position.pieces[WHITE][TOTAL] = countBits(pBoard->position.whiteOccupied);
	
	pBoard->position.pieces[BLACK][PAWN] = countBits(pBoard->position.pieceBB[BLACK_PAWN]);
	pBoard->position.pieces[BLACK][BISHOP] = countBits(pBoard->position.pieceBB[BLACK_BISHOP]);
	pBoard->position.pieces[BLACK][KNIGHT] = countBits(pBoard->position.pieceBB[BLACK_KNIGHT]);
	pBoard->position.pieces[BLACK][ROOK] = countBits(pBoard->position.pieceBB[BLACK_ROOK]);
	pBoard->position.pieces[BLACK][QUEEN] = countBits(pBoard->position.pieceBB[BLACK_QUEEN]);
	pBoard->position.pieces[BLACK][KING] = countBits(pBoard->position.pieceBB[BLACK_KING]);
	pBoard->position.pieces[BLACK][TOTAL] = countBits(pBoard->position.blackOccupied);
	
	pBoard->position.totalPieces = countBits(pBoard->position.occupied);
	return;
}

void updateKingsFromBB(Board *pBoard) {
	assert(countBits(pBoard->position.pieceBB[WHITE_KING]) == 1
			&& countBits(pBoard->position.pieceBB[BLACK_KING]) == 1);
	pBoard->position.kings[WHITE] = LSB(pBoard->position.pieceBB[WHITE_KING]);
	pBoard->position.kings[BLACK] = LSB(pBoard->position.pieceBB[BLACK_KING]);
	return;
}

void resetBB(Board *pBoard) {
	pBoard->position.pieceBB[WHITE_KING] = 0;
	pBoard->position.pieceBB[WHITE_QUEEN] = 0;
	pBoard->position.pieceBB[WHITE_ROOK] = 0;
	pBoard->position.pieceBB[WHITE_BISHOP] = 0;
	pBoard->position.pieceBB[WHITE_KNIGHT] = 0;
	pBoard->position.pieceBB[WHITE_PAWN] = 0;
	pBoard->position.blackOccupied = 0;
	
	pBoard->position.pieceBB[BLACK_KING] = 0;
	pBoard->position.pieceBB[BLACK_QUEEN] = 0;
	pBoard->position.pieceBB[BLACK_ROOK] = 0;
	pBoard->position.pieceBB[BLACK_BISHOP] = 0;
	pBoard->position.pieceBB[BLACK_KNIGHT] = 0;
	pBoard->position.pieceBB[BLACK_PAWN] = 0;
	pBoard->position.blackOccupied = 0;
	
	pBoard->position.occupied = 0;
	return;
}

void updateAggregateBB(Board *pBoard) {
	pBoard->position.whiteOccupied = pBoard->position.pieceBB[WHITE_PAWN] |
																	pBoard->position.pieceBB[WHITE_KING] |
																	pBoard->position.pieceBB[WHITE_QUEEN] |
																	pBoard->position.pieceBB[WHITE_BISHOP] |
																	pBoard->position.pieceBB[WHITE_KNIGHT] |
																	pBoard->position.pieceBB[WHITE_ROOK];

	pBoard->position.blackOccupied = pBoard->position.pieceBB[BLACK_PAWN] |
																	pBoard->position.pieceBB[BLACK_KING] |
																	pBoard->position.pieceBB[BLACK_QUEEN] |
																	pBoard->position.pieceBB[BLACK_BISHOP] |
																	pBoard->position.pieceBB[BLACK_KNIGHT] |
																	pBoard->position.pieceBB[BLACK_ROOK];
																		
	pBoard->position.occupied = pBoard->position.whiteOccupied |
															pBoard->position.blackOccupied;
	return;
}

void updateBBFromSquares(Board *pBoard) {
	int i;
	for(i = 0; i < 64; i++) {
		if(pBoard->position.square[i] == WHITE_PAWN) pBoard->position.pieceBB[WHITE_PAWN] |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_KING) pBoard->position.pieceBB[WHITE_KING] |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_QUEEN) pBoard->position.pieceBB[WHITE_QUEEN] |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_BISHOP) pBoard->position.pieceBB[WHITE_BISHOP] |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_KNIGHT) pBoard->position.pieceBB[WHITE_KNIGHT] |= BITSET[i];
		if(pBoard->position.square[i] == WHITE_ROOK) pBoard->position.pieceBB[WHITE_ROOK] |= BITSET[i];
		
		if(pBoard->position.square[i] == BLACK_PAWN) pBoard->position.pieceBB[BLACK_PAWN] |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_KING) pBoard->position.pieceBB[BLACK_KING] |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_QUEEN) pBoard->position.pieceBB[BLACK_QUEEN] |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_BISHOP) pBoard->position.pieceBB[BLACK_BISHOP] |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_KNIGHT) pBoard->position.pieceBB[BLACK_KNIGHT] |= BITSET[i];
		if(pBoard->position.square[i] == BLACK_ROOK) pBoard->position.pieceBB[BLACK_ROOK] |= BITSET[i];
	}
	return;
}

void updateMaterialFromBB(Board *pBoard) {
	pBoard->info.whiteMaterial = countBits(pBoard->position.pieceBB[WHITE_PAWN]) * PAWN_VALUE
														 + countBits(pBoard->position.pieceBB[WHITE_BISHOP]) * BISHOP_VALUE
														 + countBits(pBoard->position.pieceBB[WHITE_KNIGHT]) * KNIGHT_VALUE
														 + countBits(pBoard->position.pieceBB[WHITE_ROOK]) * ROOK_VALUE
														 + countBits(pBoard->position.pieceBB[WHITE_QUEEN]) * QUEEN_VALUE;
													
	pBoard->info.blackMaterial = countBits(pBoard->position.pieceBB[BLACK_PAWN]) * PAWN_VALUE
														 + countBits(pBoard->position.pieceBB[BLACK_BISHOP]) * BISHOP_VALUE
														 + countBits(pBoard->position.pieceBB[BLACK_KNIGHT]) * KNIGHT_VALUE
														 + countBits(pBoard->position.pieceBB[BLACK_ROOK]) * ROOK_VALUE
														 + countBits(pBoard->position.pieceBB[BLACK_QUEEN]) * QUEEN_VALUE;
														
	pBoard->info.material = pBoard->info.whiteMaterial
												- pBoard->info.blackMaterial;
	
	return;
}
