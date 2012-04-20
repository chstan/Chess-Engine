#include "defines.h"
#include "extglobals.h"
#include "board.h"
#include "protos.h"

typedef struct {
	Move moveList[MAX_MOVES_PER_PLY];
	Move killers[MAX_KILLERS_PER_PLY];
	int timidIndex;
	int totalMoves;
	int totalKillers;
	int currentIndex;
	int moveIter;
	int killerIter;
} MoveSet;

