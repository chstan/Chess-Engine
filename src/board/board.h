#ifndef BOARD_H
#define BOARD_H

#include "../defines.h"

#define sq(RANK, FILE) (8*((RANK)-1)+((FILE)-1))

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
	unsigned char castleWhite;
	unsigned char castleBlack;
	
	int enPassantSquare;
	int staleMoves;
	
	bool displayRotated;
} BoardInfo;

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

void enPassant(Board *pBoard);

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
