#ifndef PROTOS_H
#define PROTOS_H
#include "defines.h"
#include "board.h"

//-----------------UI AND CONTROL PROTOTYPES----------------
void init(void);
void dataInit(void);
void boardInit(void);
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

//-----------------------BIT OPS----------------------------
unsigned int countBits(BitMap bits);
unsigned int GSB(BitMap bits);
unsigned int LSB(BitMap bits);

#endif