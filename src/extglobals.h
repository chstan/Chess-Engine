#ifndef EXTGLOBALS_H
#define EXTGLOBALS_H

#include "defines.h"
#include "board/board.h"
#include "move/move.h"

extern char CMD_BUFFER[];
extern int CMD_BUFFER_COUNT;
extern char *TOKENS[];
extern int TOKEN_COUNT;

extern Board *pBoard;

extern const int INVALID_SQUARE;
extern const int A8;  extern const int B8;  extern const int C8;  extern const int D8; 
extern const int E8;  extern const int F8;  extern const int G8;  extern const int H8; 
extern const int A7;  extern const int B7;  extern const int C7;  extern const int D7; 
extern const int E7;  extern const int F7;  extern const int G7;  extern const int H7; 
extern const int A6;  extern const int B6;  extern const int C6;  extern const int D6; 
extern const int E6;  extern const int F6;  extern const int G6;  extern const int H6; 
extern const int A5;  extern const int B5;  extern const int C5;  extern const int D5; 
extern const int E5;  extern const int F5;  extern const int G5;  extern const int H5; 
extern const int A4;  extern const int B4;  extern const int C4;  extern const int D4; 
extern const int E4;  extern const int F4;  extern const int G4;  extern const int H4; 
extern const int A3;  extern const int B3;  extern const int C3;  extern const int D3; 
extern const int E3;  extern const int F3;  extern const int G3;  extern const int H3; 
extern const int A2;  extern const int B2;  extern const int C2;  extern const int D2; 
extern const int E2;  extern const int F2;  extern const int G2;  extern const int H2; 
extern const int A1;  extern const int B1;  extern const int C1;  extern const int D1; 
extern const int E1;  extern const int F1;  extern const int G1;  extern const int H1;

extern const char* SQUARENAME[];
 
extern const int FILES[];
extern const int RANKS[];

extern const BitBoard fullBoard;
extern const BitBoard emptyBoard;

extern BitBoard rightMove[];
extern BitBoard upMove[];
extern BitBoard leftMove[];
extern BitBoard downMove[];

extern BitBoard rightUpMove[];
extern BitBoard leftUpMove[];
extern BitBoard leftDownMove[];
extern BitBoard rightDownMove[];

extern BitBoard knightMove[];
extern BitBoard pawnMoveWhite[];
extern BitBoard pawnDoubleWhite[];
extern BitBoard pawnCaptureWhite[];
extern BitBoard pawnPromotionWhite[];
extern BitBoard pawnPromotionCaptureWhite[];
extern BitBoard pawnMoveBlack[];
extern BitBoard pawnDoubleBlack[];
extern BitBoard pawnCaptureBlack[];
extern BitBoard pawnPromotionBlack[];
extern BitBoard pawnPromotionCaptureBlack[];
extern BitBoard kingMove[];

extern const unsigned char WHITE;  
extern const unsigned char BLACK;

extern const unsigned char PAWN;
extern const unsigned char BISHOP;
extern const unsigned char KNIGHT;
extern const unsigned char ROOK;
extern const unsigned char QUEEN;
extern const unsigned char KING;
extern const unsigned char TOTAL;  
 
extern const unsigned char EMPTY;
extern const unsigned char WHITE_KNIGHT;
extern const unsigned char WHITE_PAWN;
extern const unsigned char WHITE_KING;
extern const unsigned char WHITE_BISHOP;
extern const unsigned char WHITE_ROOK;
extern const unsigned char WHITE_QUEEN; 
extern const unsigned char BLACK_KNIGHT; 
extern const unsigned char BLACK_PAWN; 
extern const unsigned char BLACK_KING; 
extern const unsigned char BLACK_BISHOP; 
extern const unsigned char BLACK_ROOK; 
extern const unsigned char BLACK_QUEEN;
 
extern const char* PIECENAMES[];
 
extern BitMap BITSET[];
extern int BOARDINDEX[9][9]; 
 
extern const int PAWN_VALUE;
extern const int KNIGHT_VALUE;
extern const int BISHOP_VALUE; 
extern const int ROOK_VALUE;
extern const int QUEEN_VALUE;
extern const int KING_VALUE;
extern const int CHECK_MATE;

extern const int PIECEVALUES[];
 
extern int GS1B[]; 
 
extern unsigned char CAN_CASTLE_OO;
extern unsigned char CAN_CASTLE_OOO;
extern unsigned char WHITE_CAN_CASTLE;
extern unsigned char BLACK_CAN_CASTLE;

extern bool randomInitialized;

#endif