#ifndef GLOBALS_H
#define GLOBALS_H

#include "defines.h"
#include "extglobals.h"
#include "board/board.h"


char CMD_BUFFER[MAX_CMD_BUFFER];
int CMD_BUFFER_COUNT = 0;
char *TOKENS[MAX_TOKENS];
int TOKEN_COUNT;

Board *pBoard;

const int INVALID_SQUARE = -1;
const int A1 =  0; const int B1 =  1; const int C1 =  2; const int D1 =  3;
const int E1 =  4; const int F1 =  5; const int G1 =  6; const int H1 =  7;
const int A2 =  8; const int B2 =  9; const int C2 = 10; const int D2 = 11;
const int E2 = 12; const int F2 = 13; const int G2 = 14; const int H2 = 15;
const int A3 = 16; const int B3 = 17; const int C3 = 18; const int D3 = 19;
const int E3 = 20; const int F3 = 21; const int G3 = 22; const int H3 = 23;
const int A4 = 24; const int B4 = 25; const int C4 = 26; const int D4 = 27;
const int E4 = 28; const int F4 = 29; const int G4 = 30; const int H4 = 31;
const int A5 = 32; const int B5 = 33; const int C5 = 34; const int D5 = 35;
const int E5 = 36; const int F5 = 37; const int G5 = 38; const int H5 = 39;
const int A6 = 40; const int B6 = 41; const int C6 = 42; const int D6 = 43;
const int E6 = 44; const int F6 = 45; const int G6 = 46; const int H6 = 47;
const int A7 = 48; const int B7 = 49; const int C7 = 50; const int D7 = 51;
const int E7 = 52; const int F7 = 53; const int G7 = 54; const int H7 = 55;
const int A8 = 56; const int B8 = 57; const int C8 = 58; const int D8 = 59;
const int E8 = 60; const int F8 = 61; const int G8 = 62; const int H8 = 63;

const char* SQUARENAME[64] = {
	"a1","b1","c1","d1","e1","f1","g1","h1",
  "a2","b2","c2","d2","e2","f2","g2","h2",
  "a3","b3","c3","d3","e3","f3","g3","h3",
  "a4","b4","c4","d4","e4","f4","g4","h4",
  "a5","b5","c5","d5","e5","f5","g5","h5",
  "a6","b6","c6","d6","e6","f6","g6","h6",
  "a7","b7","c7","d7","e7","f7","g7","h7",
  "a8","b8","c8","d8","e8","f8","g8","h8"
};

const int FILES[64] = {
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8
};

const int RANKS[64] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8
};

const BitBoard fullBoard = -1LL;
const BitBoard emptyBoard = 0LL;

BitBoard rankBB[8];
BitBoard fileBB[8];

BitBoard rightMove[64];
BitBoard upMove[64];
BitBoard leftMove[64];
BitBoard downMove[64];

BitBoard rightUpMove[64];
BitBoard leftUpMove[64];
BitBoard leftDownMove[64];
BitBoard rightDownMove[64];

BitBoard knightMove[64];
BitBoard pawnMoveWhite[64];
BitBoard pawnDoubleWhite[64];
BitBoard pawnCaptureWhite[64];
BitBoard pawnPromotionWhite[64];
BitBoard pawnPromotionCaptureWhite[64];
BitBoard pawnMoveBlack[64];
BitBoard pawnDoubleBlack[64];
BitBoard pawnCaptureBlack[64];
BitBoard pawnPromotionBlack[64];
BitBoard pawnPromotionCaptureBlack[64];
BitBoard kingMove[64];

const unsigned char WHITE = 0;
const unsigned char BLACK = 1;

const unsigned char PAWN = 0;
const unsigned char BISHOP = 1;
const unsigned char KNIGHT = 2;
const unsigned char ROOK = 3;
const unsigned char QUEEN = 4;
const unsigned char KING = 5;
const unsigned char TOTAL = 6;

																							// binary representations
const unsigned char EMPTY = 0;                //  0000
const unsigned char WHITE_PAWN = 1;           //  0001
const unsigned char WHITE_KING = 2;           //  0010
const unsigned char WHITE_KNIGHT = 3;         //  0011
const unsigned char WHITE_BISHOP =  5;        //  0101
const unsigned char WHITE_ROOK = 6;           //  0110
const unsigned char WHITE_QUEEN = 7;          //  0111
const unsigned char BLACK_PAWN = 9;           //  1001
const unsigned char BLACK_KING = 10;          //  1010
const unsigned char BLACK_KNIGHT = 11;        //  1011
const unsigned char BLACK_BISHOP = 13;        //  1101
const unsigned char BLACK_ROOK = 14;          //  1110
const unsigned char BLACK_QUEEN = 15;         //  1111

const char *PIECE_NAMES[16] = {"   ", " P ", " K ", " N ", "   ", " B ", " R ", " Q ",
															 "   ", "*P*", "*K*", "*N*", "   ", "*B*", "*R*", "*Q*"};

const char *PIECE_NAMES_FULL[16] = {"empty square", "white pawn", "white king", "white knight", "N/A", "white bishop", "white rook", "white queen",
																    "empty square", "black pawn", "black king", "black knight", "N/A", "black bishop", "black rook", "black queen"};

int BOARDINDEX[9][9];

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 325;
const int BISHOP_VALUE = 325;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 1050;
const int KING_VALUE = 90000;
const int CHECK_MATE = 90000;

int const PIECEVALUES[16] =
	{0, 100, 90000, 325, 0, 325, 500, 1050,
	 0, 100, 90000, 325, 0, 325, 500, 1050}; 

int GS1B[256];

bool randomInitialized = false;

#endif
