#ifndef ENGINE_DEFINES_H
#define ENGINE_DEFINES_H

#include <stdbool.h>

#define ENGINE_VERSION "Engine 0.0.0"
#define MAX_CMD_BUFFER 256

#define W 0
#define B 1
#define color(p) ((p & 8) == 8)

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