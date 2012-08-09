#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "movegentest.h"
#include "../move/movegen.h"



void performanceTest(Board *pBoard, int depth) {
	MoveCount count;
	memset(&count, 0, sizeof(MoveCount));
	int startTime = clock();
	U64 totalMoves = recursiveMoveCount(pBoard, depth, &count);
	int endTime = clock();
	double seconds = ((double)(endTime - startTime))/CLOCKS_PER_SEC;
	printf("Performance test results:\n%llu move(s) generated in\n%f seconds.\n", totalMoves, seconds);
}

U64 recursiveMoveCount(Board *pBoard, int depth, MoveCount *pCount) {
	MoveSet moves;
	U64 countedMoves = 0;
	resetMoveSet(&moves);
	
	if(depth == 0) return 1;
	
	// now find all the nodes immediately off the current board state
	generateMove(pBoard, &moves);
	
	if(depth == 1) return moves.totalMoves;
	
	for(int i = 0; i < moves.totalMoves; i++) {
		makeMove(pBoard, moves.moveList[i]);
		if()
		countedMoves += recursiveMoveCount(pBoard, depth-1);
		unmakeMove(pBoard, moves.moveList[i]); 
	}
	
	return countedMoves;
}
