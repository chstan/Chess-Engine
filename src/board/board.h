#ifndef BOARD_H
#define BOARD_H

#include "../defines.h"

#define sq(RANK, FILE) (8*((RANK)-1)+((FILE)-1))
#define MAX_MOVES_PER_GAME 512


typedef struct {
	BitBoard pieceBB[16];
	
	BitBoard whiteOccupied;
	BitBoard blackOccupied;
	BitBoard occupied;
	
	int square[64];
	int kings[2];
	
	signed char pieces[16];
	signed char totalWhite;
	signed char totalBlack;
	signed char totalPieces;
} Position;

typedef struct {
	int material;
	int whiteMaterial;
	int blackMaterial;
	
	unsigned char nextMove;
	MoveInfo state[MAX_MOVES_PER_GAME];

	// indexs into the array, gives us the number
	// of moves that have elapsed so far
	unsigned int currentMove;	

	bool displayRotated;
} BoardInfo;

typedef struct {
	unsigned char castleWhite;
	unsigned char castleBlack;
	
	// the en passant index
	// represents where a pawn can capture to, rather than
	// where the pawn actually is
	int enPassantSquare;
	int staleMoves;
} MoveInfo;

typedef struct {
	Position position;
	BoardInfo info;
} Board;

//--------------------RESET AND DISPLAY----------------------

void resetBoard(Board *pBoard);

void initBoardFromSquares(Board* pBoard, unsigned char nextMove, int staleMoves, int castleW, 
	int castleB, int enPassantSquare);

void displayBoard(Board *pBoard);

void rotateBoard(Board *pBoard);

//------------------------DEBUG------------------------------
void debugBoard(Board *pBoard);

//---------------------MAKE FUNCTIONS------------------------
void setEmptyAt(Board *pBoard, UCHAR index, UCHAR lastOccupant);

void setPieceAt(Board *pBoard, UCHAR index, UCHAR movedPiece, UCHAR capturedPiece);

void updateEnPassantSquare(Board *pBoard, int index);

void enPassant(Board *pBoard, int color);

void unPassant(Board *pBoard, int index, int color);

void updateCastling(Board *pBoard, int index, UCHAR piece);

void castle(Board *pBoard, UCHAR index, UCHAR whichKing);

void unCastle(Board *pBoard, UCHAR index, UCHAR whichKing);

void doPromote(Board *pBoard, UCHAR index, UCHAR whichPromote);

void unPromote(Board *pBoard, UCHAR index);

void addMaterial(Board *pBoard, UCHAR piece);

void removeMaterial(Board *pBoard, UCHAR piece);

//---------------------AUX FUNCTIONS-------------------------
void updatePieceCountsFromBB(Board *pBoard);

void updateKingsFromBB(Board *pBoard);

void resetBB(Board *pBoard);

void updateAggregateBB(Board *pBoard);

void updateBBFromSquares(Board *pBoard);

void updateMaterialFromBB(Board *pBoard);


#endif
