#ifndef EXTGLOBALS_H
#define EXTGLOBALS_H

#include "defines.h"
#include "board/board.h"
#include "move/move.h"

extern char CMD_BUFFER[];
extern int CMD_BUFFER_COUNT;
extern char *TOKENS[];
extern int TOKEN_COUNT;

extern Board *p_board;

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

extern const U64 fullBoard;
extern const U64 emptyBoard;

extern U64 rankBB[];
extern U64 fileBB[];

extern U64 rightMove[];
extern U64 upMove[];
extern U64 leftMove[];
extern U64 downMove[];

extern U64 rightUpMove[];
extern U64 leftUpMove[];
extern U64 leftDownMove[];
extern U64 rightDownMove[];

extern U64 knightMove[];
extern U64 pawnMoveWhite[];
extern U64 pawnDoubleWhite[];
extern U64 pawnCaptureWhite[];
extern U64 pawnAllCaptureWhite[];
extern U64 pawnPromotionWhite[];
extern U64 pawnPromotionCaptureWhite[];
extern U64 pawnMoveBlack[];
extern U64 pawnDoubleBlack[];
extern U64 pawnCaptureBlack[];
extern U64 pawnAllCaptureBlack[];
extern U64 pawnPromotionBlack[];
extern U64 pawnPromotionCaptureBlack[];
extern U64 kingMove[];

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

extern const char *PIECE_NAMES[];
extern const char *PIECE_NAMES_FULL[];

extern const int PAWN_VALUE;
extern const int KNIGHT_VALUE;
extern const int BISHOP_VALUE;
extern const int ROOK_VALUE;
extern const int QUEEN_VALUE;
extern const int KING_VALUE;
extern const int CHECK_MATE;

extern const int PIECEVALUES[];

extern int GS1B[];

extern bool randomInitialized;

#endif
