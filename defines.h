#ifndef ENGINE_DEFINES_H
#define ENGINE_DEFINES_H

#include <stdbool.h>

#define ENGINE_VERSION "Engine 0.0.0"
#define MAX_CMD_BUFFER 256
#define MAX_TOKENS 8

#define MAX_MOVES_PER_PLY 256
#define MAX_KILLERS 2

#define W 0
#define B 1
#define color(p) ((p & 8) == 8)

#define E 0
#define WP 1
#define BP 9
#define WK 2
#define BK 10
#define WN 3
#define BN 11
#define WB 5
#define BB 13
#define WR 6
#define BR 14
#define WQ 7
#define BQ 15

typedef unsigned long long U64;
typedef unsigned long long BitMap;
typedef short SHORTINT;
typedef unsigned short USHORTINT;

typedef unsigned long long BitBoard;
typedef unsigned char UCHAR;

enum ALERTS {
	UNKNOWN_COMMAND
};

#endif