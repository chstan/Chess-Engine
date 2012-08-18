#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "movegentest.h"
#include "../move/notation.h"
#include "../move/movegen.h"
#include "../defines.h"

static void doPerfTest(Board *pBoard, int depth);

void performanceTest(Board *pBoard, int depth, bool divide) {
	if (depth < 1) return;
	
	if (divide)
		printf("%-8s ", "Move");
	printf("%-16s %-8s", "# Moves", "Time (s)");
	#ifdef DEBUG
	printf(" %-8s %-8s %-8s %-8s %-8s", "Caps", "EPs", "Castles", "Proms", "Checks");
	#endif
	printf("\n");
	
	if (divide){
		MoveSet moves;
		resetMoveSet(&moves);
		generateMove(pBoard, &moves);
		
		for (int i = 0; i < moves.totalMoves; i++) {
			Move m = moves.moveList[i];
			char *notation = moveToNotation(pBoard, m);
			printf("%-8s ", notation);
			free(notation);
			
			makeMove(pBoard, m);
			doPerfTest(pBoard, depth-1);
			unmakeMove(pBoard, m);
		}
	} else {
		doPerfTest(pBoard, depth);
	}
}

static void doPerfTest(Board *pBoard, int depth) {
	MoveCount count;
	memset(&count, 0, sizeof(MoveCount));
	int startTime = clock();
	U64 totalMoves = recursiveMoveCount(pBoard, depth, &count);
	int endTime = clock();
	double seconds = ((double)(endTime - startTime))/CLOCKS_PER_SEC;
	printf("%-16llu %-8.3f", totalMoves, seconds);
	#ifdef DEBUG
	printf(" %-8llu %-8llu %-8llu %-8llu %-8llu", count.captures,
		count.enPassants, count.castles, count.promotions, count.checks);
	#endif
	printf("\n");
}

U64 recursiveMoveCount(Board *pBoard, int depth, MoveCount *pCount) {
	MoveSet moves;
	U64 countedMoves = 0;
	resetMoveSet(&moves);
	
	if(depth == 0) return 1;
	
	// now find all the nodes immediately off the current board state
	generateMove(pBoard, &moves);
	
	if(depth == 1) {

		U64 count = 0;
		for (int i = 0; i < moves.totalMoves; i++) {
			Move m = moves.moveList[i];
			makeMove(pBoard, m);
			if (!checks(pBoard, otherColor(pBoard->info.toPlay))) {
				count++;
				#ifdef DEBUG
				if (capturedPiece(m)) pCount->captures++;
				if (whiteEnPassant(m) || blackEnPassant(m)) pCount->enPassants++;
				if (promote(m)) pCount->promotions++;
				if (whiteCastle(m) || blackCastle(m)) pCount->castles++;
				if (checks(pBoard, pBoard->info.toPlay)) pCount->checks++;
				#endif
			}
			unmakeMove(pBoard, m);
		}
		return count;
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
