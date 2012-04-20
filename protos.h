#ifndef PROTOS_H
#define PROTOS_H
#include "defines.h"
#include "board.h"
#include "move.h"
#include "movegen.h"

//-----------------INITIALIZATION PROTOTYPES----------------
void init(void);
void dataInit(void);
void boardInit(void);
void moveTableInit(void);

//-----------------UI AND CONTROL PROTOTYPES----------------
bool doCommand(void);
void readCommands(void);
bool tokenizeCommand(void);
void alert(int ALERT);
void prepareForExit(void);

//------------------------NOTATION--------------------------
Move notationToMove(Board *pBoard);

//--------------------MAKE/UNMAKE MOVE----------------------
void makeMove(Board *pBoard, Move m);
void unmakeMove(Board *pBoard, Move m);

void printMove(Move m);

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

void setEmptyAt(Board *pBoard, UCHAR index, UCHAR lastOccupant);
void setPieceAt(Board *pBoard, UCHAR index, UCHAR movedPiece, UCHAR capturedPiece);

void enPassant(Board *pBoard);

void castle(Board *pBoard, UCHAR index, UCHAR whichKing);
void unCastle(Board *pBoard, UCHAR index, UCHAR whichKing);

void doPromote(Board *pBoard, UCHAR index, UCHAR whichPromote);
void unPromote(Board *pBoard, UCHAR index);

void addMaterial(Board *pBoard, UCHAR piece);
void removeMaterial(Board *pBoard, UCHAR piece);

//-----------------------BIT OPS----------------------------
unsigned int countBits(BitMap bits);
unsigned int GSB(BitMap bits);
unsigned int LSB(BitMap bits);
void printBitMap(BitMap bits);

//-----------------------MOVEGEN----------------------------
void generateTimid(Board *pBoard, MoveSet *pMoves);
void generateCapture(Board *pBoard, MoveSet *pMoves);
void generateCheck(Board *pBoard, MoveSet *pMoves);
void generateMove(Board *pBoard, MoveSet *pMoves);

BitBoard attacks(Board *pBoard, int attackeeIndex, int side);
BitBoard checks(Board *pBoard, int side);

void extractMoves(Board *pBoard);

void resetMoveSet(MoveSet *pMoves);
void initializeMoveSet(Board *pBoard, MoveSet *pMoves); 
Move nextMove(MoveSet *pMoves);
void writeMove(MoveSet *pMoves, Move m);

//-------------------MOVEGEN HELPERS------------------------
BitBoard pawnPromotion(Board *pBoard, UCHAR pawnIndex, int side);
BitBoard pawnPromotionCaptureBB(Board *pBoard, UCHAR pawnIndex, int side);

BitBoard castleBB(Board *pBoard, UCHAR kingIndex, int side);

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