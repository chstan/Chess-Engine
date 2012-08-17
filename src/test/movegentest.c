#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "movegentest.h"
#include "../move/notation.h"
#include "../move/movegen.h"
#include "../defines.h"

void divide(Board *pBoard, int depth) {
	if(depth < 1) return;
	
	MoveSet moves;
	resetMoveSet(&moves);
	generateMove(pBoard, &moves);
	
	for(int i = 0; i < moves.totalMoves; i++) {
		Move m = moves.moveList[i];
		char *notation = moveToNotation(pBoard, m);
		printf("%s\t", notation);
		free(notation);
		
		makeMove(pBoard, m);
		performanceTest(pBoard, depth-1);
		unmakeMove(pBoard, m);
		//printf("\n");
	}
}

void performanceTest(Board *pBoard, int depth) {
	MoveCount count;
	memset(&count, 0, sizeof(MoveCount));
	int startTime = clock();
	U64 totalMoves = recursiveMoveCount(pBoard, depth, &count);
	int endTime = clock();
	double seconds = ((double)(endTime - startTime))/CLOCKS_PER_SEC;
	#ifdef DEBUG
	printf("Performance test results:\n%llu move(s) generated in\n%f seconds.\n", totalMoves, seconds);
	printf("\tCaptures   : %llu\n", count.captures);
	printf("\tEP         : %llu\n", count.enPassants);
	printf("\tCastles    : %llu\n", count.castles);
	printf("\tPromotions : %llu\n", count.promotions);
	printf("\tChecks     : %llu\n", count.checks);
	#else
	printf("%llu\t%f\n", totalMoves, seconds);
	#endif
}

U64 recursiveMoveCount(Board *pBoard, int depth, MoveCount *pCount) {
	MoveSet moves;
	U64 countedMoves = 0;
	resetMoveSet(&moves);
	
	if(depth == 0) return 1;
	
	// now find all the nodes immediately off the current board state
	generateMove(pBoard, &moves);
	
	if(depth == 1) {
		#ifdef DEBUG
		for (int i = 0; i < moves.totalMoves; i++) {
			Move m = moves.moveList[i];
			makeMove(pBoard, m);
			if (!checks(pBoard, otherColor(pBoard->info.toPlay))) {
				if (capturedPiece(m)) pCount->captures++;
				if (whiteEnPassant(m) || blackEnPassant(m)) pCount->enPassants++;
				if (promote(m)) pCount->promotions++;
				if (whiteCastle(m) || blackCastle(m)) pCount->castles++;
				if (checks(pBoard, pBoard->info.toPlay)) pCount->checks++;
			}
			unmakeMove(pBoard, m);
		}
		#endif
		return moves.totalMoves;
	}
	
	for(int i = 0; i < moves.totalMoves; i++) {
		Move m = moves.moveList[i];
		makeMove(pBoard, m);
		if (!checks(pBoard, otherColor(pBoard->info.toPlay))) {
			countedMoves += recursiveMoveCount(pBoard, depth-1, pCount);
		}
		unmakeMove(pBoard, m); 
	}
	return countedMoves;
}
