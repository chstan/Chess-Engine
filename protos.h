#ifndef PROTOS_H
#define PROTOS_H
#include "defines.h"

void init(void);
void dataInit(void);
void boardInit(void);
bool do_command(const char *cmd);
void read_commands(void);
void alert(int ALERT);

//--------------------BOARD PROTOTYPES----------------------
void resetBoard(Board *pBoard);
void initBoardFromSquares(Board *pBoard, unsigned char next,
	int stale, int castleW, int castleB, int enPSq);
void displayBoard(Board *pBoard);

//-----------------------BIT OPS----------------------------
unsigned int countBits(BitMap bits);
unsigned int GSB(BitMap bits);
unsigned int LSB(BitMap bits);

#endif