#include "defines.h"
#include "extglobals.h"
#include "board.h"
#include "protos.h"

typedef struct {
	Move moveList[MAX_MOVES_PER_PLY];
	Move killers[MAX_KILLERS];
	int timidIndex;
	int totalMoves;
	int totalKillers;
	int currentIndex;
} MoveSet;

