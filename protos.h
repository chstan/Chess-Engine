#ifndef PROTOS_H
#define PROTOS_H
#include "defines.h"
#include "board.h"

//-----------------INITIALIZATION PROTOTYPES----------------
void init(void);
void dataInit(void);
void boardInit(void);
void moveTableInit(void);

//-----------------UI AND CONTROL PROTOTYPES----------------
bool doCommand(const char *cmd);
void readCommands(void);
void alert(int ALERT);
void prepareForExit(void);

//--------------------BOARD PROTOTYPES----------------------
void resetBoard(Board *pBoard);
void initBoardFromSquares(Board *pBoard, unsigned char nextMove,
	int staleMoves, int castleW, int castleB, int enPassantSquare);
void displayBoard(Board *pBoard);
void updateAggregateBB(Board *pBoard);
void updateMaterialFromBB(Board *pBoard);
void resetBB(Board *pBoard);
void updateBBFromSquares(Board *pBoard);
void updateKingsFromBB(Board *pBoard);
void updatePieceCountsFromBB(Board *pBoard);
void displayBoard(Board *pBoard);
void rotateBoard(Board *pBoard);

//-----------------------BIT OPS----------------------------
unsigned int countBits(BitMap bits);
unsigned int GSB(BitMap bits);
unsigned int LSB(BitMap bits);
void printBitMap(BitMap bits);

//-------------------MOVEGEN HELPERS------------------------
bool pawnPromotion(Board *pBoard, UCHAR pawnIndex, int side);

BitBoard pawnPromotionCaptureBB(Board *pBoard, UCHAR pawnIndex, int side);

BitBoard knightTimidBB(Board *pBoard, UCHAR knightIndex, int side);
BitBoard bishopTimidBB(Board *pBoard, UCHAR bishopIndex, int side);
BitBoard queenTimidBB(Board *pBoard, UCHAR queenIndex, int side);
BitBoard kingTimidBB(Board *pBoard, UCHAR kingIndex, int side);
BitBoard pawnTimidBB(Board *pBoard, UCHAR pawnIndex, int side);
BitBoard rookTimidBB(Board *pBoard, UCHAR rookIndex, int side);

BitBoard knightCaptureBB(Board *pBoard, UCHAR knightIndex, int side);
BitBoard bishopCaptureBB(Board *pBoard, UCHAR bishopIndex, int side);
BitBoard queenCaptureBB(Board *pBoard, UCHAR queenIndex, int side);
BitBoard kingCaptureBB(Board *pBoard, UCHAR kingIndex, int side);
BitBoard pawnCaptureBB(Board *pBoard, UCHAR pawnIndex, int side);
BitBoard rookCaptureBB(Board *pBoard, UCHAR rookIndex, int side);

BitBoard knightMoveBB(Board *pBoard, UCHAR knightIndex, int side);
BitBoard bishopMoveBB(Board *pBoard, UCHAR bishopIndex, int side);
BitBoard queenMoveBB(Board *pBoard, UCHAR queenIndex, int side);
BitBoard kingMoveBB(Board *pBoard, UCHAR kingIndex, int side);
BitBoard pawnMoveBB(Board *pBoard, UCHAR pawnIndex, int side);
BitBoard rookMoveBB(Board *pBoard, UCHAR rookIndex, int side);

#endif