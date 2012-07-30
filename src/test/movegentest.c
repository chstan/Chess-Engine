#include <time.h>
#include <stdio.h>

#include "movegentest.h"
#include "../move/movegen.h"

void performanceTest(Board *pBoard, int depth) {
	int startTime = clock();
	U64 totalMoves = recursiveMoveCount(pBoard, depth);
	int endTime = clock();
	printf("Performance test results:\n%llu move(s) generated in\n%d cycles.\n", totalMoves, endTime-startTime);
}

U64 recursiveMoveCount(Board *pBoard, int depth) {
	MoveSet moves;
	U64 countedMoves = 0;
	resetMoveSet(&moves);
	
	if(depth == 0) return 1;
	
	// now find all the nodes immediately off the current board state
	generateMove(pBoard, &moves);
	
	if(depth == 1) return moves.totalMoves;
	
	for(int i = 0; i < moves.totalMoves; i++) {
		makeMove(pBoard, moves.moveList[i]);
		countedMoves += recursiveMoveCount(pBoard, depth-1);
		unmakeMove(pBoard, moves.moveList[i]); 
	}
	
	return countedMoves;
}
