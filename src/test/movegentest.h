#ifndef MOVEGENTEST_H
#define MOVEGENTEST_H

#include "../defines.h"
#include "../move/move.h"
#include "../move/movegen.h"

typedef struct {
	U64 moves;
	U64 captures;
	U64 enPassants;
	U64 promotions;
	U64 checks;
	U64 castles;
} MoveCount;


/* (was function divide, now part of performanceTest)
 *  ----------------
 * Generates all possible moves at the starting node. For each generated node, divide prints
 * the move in algebraic notation, and runs performanceTest with depth-1, allowing us to easily
 * isolate bugs in the move generator.
**/
/** Function: performanceTest
 *  -------------------------
 * Times the move generator up to a depth specified
 * in half moves (ply). It prints the number of possible moves
 * in this restricted game tree, as well as the time it took the
 * internal move generator to reach and count these possible states
**/
void performanceTest(Board *pBoard, int depth, bool divide);

/** Function: recursiveMoveCount
 *  ----------------------------
 * This function simply counts the moves up to a specified depth in half moves,
 * performanceTest (see above), times this function to test the accuracy and speed
 * of the move generator. The return is an unsigned 64 bit type, to avoid any integer overflows,
 * though generating more than 4 billion or so moves would probably take an unreasonably
 * long time anyway.
**/
U64 recursiveMoveCount(Board *pBoard, int depth, MoveCount *pCount);

#endif
