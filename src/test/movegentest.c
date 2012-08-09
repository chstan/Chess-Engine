#include <time.h>
#include <stdio.h>
#include <string.h>

#include "movegentest.h"
#include "../move/movegen.h"
#include "../defines.h"


void performanceTest(Board *pBoard, int depth) {
	MoveCount count;
	memset(&count, 0, sizeof(MoveCount));
	int startTime = clock();
	U64 totalMoves = recursiveMoveCount(pBoard, depth, &count);
	int endTime = clock();
	double seconds = ((double)(endTime - startTime))/CLOCKS_PER_SEC;
	printf("Performance test results:\n%llu move(s) generated in\n%f seconds.\n", totalMoves, seconds);
	#ifdef DEBUG
	printf("Captures   : %llu\n", count.captures);
	printf("EP         : %llu\n", count.enPassants);
	printf("Castles    : %llu\n", count.castles);
	printf("Promotions : %llu\n", count.promotions);
	printf("Checks     : %llu\n", count.checks);
	#endif
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
		if(!checks(pBoard, pBoard->position.kings[(pBoard->info.toPlay == WHITE) ? BLACK : WHITE])) {
			countedMoves += recursiveMoveCount(pBoard, depth-1, pCount);
			#ifdef DEBUG
			if(depth == 1) {
				Move m = moves.moveList[i];
				if(capturedPiece(m)) pCount->captures++;
				if(whiteEnPassant(m) || blackEnPassant(m)) pCount->enPassants++;
				if(promote(m)) pCount->promotions++;
				if(whiteCastle(m) || blackCastle(m)) pCount->castles++;
				if(checks(pBoard, pBoard->position.kings[pBoard->info.toPlay])) pCount->checks++;
			}
			#endif
		}
		unmakeMove(pBoard, moves.moveList[i]); 
	}
	return countedMoves;
}
