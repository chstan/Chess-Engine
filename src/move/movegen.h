#include "../defines.h"
#include "../extglobals.h"
#include "../board/board.h"

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

// void generateCheck(Board *pBoard);
 
// BitBoard attacks(Board *pBoard, int attackeeIndex);
// BitBoard checks(Board *pBoard, int kingIndex);

// void extractMoves(Board *pBoard);


void generateAgnostic(Board *pBoard, int color, BitBoard currentPieces, 
	int piece, MoveSet *pMoves, BitBoard (*moveGen)(Board *pBoard, UCHAR origin, int color));
	
void generateTimid(Board *pBoard, MoveSet *pMoves);

void generateCapture(Board *pBoard, MoveSet *pMoves);

void generateMove(Board *pBoard, MoveSet *pMoves);

void initializeMoveSet(Board *pBoard, MoveSet *pMoves);

void initializeMoveSetQuiet(Board *pBoard, MoveSet *pMoves);

/**
 *\brief resetMoveSet
 *
 *	resetMoveSet is an auxilliary function
 *  which effectively clears the state of the
 *  moveSet passed in as the parameter.
 *
 *\param  Pointer to the moveset
 *\return (void)
**/
void resetMoveSet(MoveSet *pMoves);

/**
 *\brief nextMove
 *
 *	nextMove returns the next unread move from the
 *  moveSet and updates the iterator to point to the
 *  next spot in the array. It also asserts that you are not
 *  trying to read an unwritten move from the moveSet.
 *
 *\param  Pointer to the moveSet
 *\return Move that was read from the moveSet
**/
Move nextMove(MoveSet *pMoves);

/**
 *\brief writeMove
 *
 *	writeMove adds a non-killer move to a moveSet
 *  and leaves the moveSet in a consistent state, updating
 *  iterators as necessary.
 *
 *\param  Pointer to the moveSet
 *\param  Move to be written
 *\return (void)
**/
void writeMove(MoveSet *pMoves, Move m);

/**
 *\brief writeKiller
 *
 *	writeKiller adds a killer move to a moveSet
 *  and leaves the moveSet in a consistent state, updating
 *  iterators as necessary.
 *
 *\param  Pointer to the moveSet
 *\param  Killer move to be written
 *\return (void)
**/
void writeKiller(MoveSet *pMoves, Move killer);

//===============UNFINISHED===================

void generateCheck(Board *pBoard, MoveSet *pMoves);

BitBoard attacks(Board *pBoard, int attackeeIndex, int side);

BitBoard checks(Board *pBoard, int side);



