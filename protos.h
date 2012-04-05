#ifndef PROTOS_H
#define PROTOS_H
#include "defines.h"

void init(void);
bool do_command(const char *cmd);
void read_commands(void);
void alert(int ALERT);

//--------------------BOARD PROTOTYPES----------------------
void resetBoard(Board *b);
void resetBoardFromSquares(Board *b, int inputSquares[64],
	unsigned char next, int stale, int castleW, int castleB, int enPSq);
void displayBoard(Board *b);

//-----------------------BIT OPS----------------------------
unsigned int countBits(BitMap bits);
unsigned int GSB(BitMap bits);
unsigned int LSB(BitMap bits);

#endif