#include "defines.h"
#include "extglobals.h"
#include "board.h"
#include "protos.h"

typedef struct {
	Move moveList[MAX_MOVES_PER_PLY];
	Move killerList[MAX_KILLERS_PER_PLY];
	int timidIndex;
	int totalMoves;
	int moveIter;
	int currentMoveIndex;
	
	int totalKillers;
	int currentKillerIndex;
	int killerIter;
} MoveSet;

