#include <stdio.h>
#include <stdlib.h>

#include "commandlist.h"
#include "defines.h"
#include "extglobals.h"
#include "alert.h"
#include "move/move.h"
#include "move/movegen.h"
#include "move/notation.h"
#include "aux/random.h"
#include "test/movegentest.h"
#include "pgnfile.h"

CommandCB commandCallbacks[] = {
	listMovesCommand,
	randomMoveCommand,
	performanceTestCommand,
	helpCommand,
	moveCommand,
	aboutCommand,
	displayCommand,
	loadpgnCommand,
	debugCommand,
	rotateCommand,
	todoCommand,
	quitCommand
};

char *commandTriggers[] = {
	"list-moves",
	"random-move",
	"performance-test",
	"help",
	"move",
	"about",
	"display",
	"load-pgn",
	"debug-board",
	"rotate",
	"todo",
	"quit"
};

char *commandDescription[] = {
	"list-moves: generates all the moves from the current board position and display them.",
	"random-move: generates all the moves from the current board position and applies one to the board.",
	"performance-test: will print the time to generate and the number of generated positions to a specified dpeth.",
	"help: prints the list of all commands.",
	"move: applies a move to the board using the following format:\n"
		"\t[PIECE] [ORIGIN] [DESTINATION] [CAPT] [PROMO] or [CASTLE]",
	"about: prints information about the chess engine",
	"display: prints the board state along with certain internal information",
	"load-pgn: loads a .pgn file specified by [FILENAME]\n",
	"debug-board: runs an internal board consistency check. No results are good results.",
	"rotate: rotates the board when displaying.",
	"todo: prints a todo list for the developers, largely unnecessary now they project is on Github",
	"quit: quits from the chess engine back to your ::1ly terminal"
};

bool listMovesCommand(int tokenCount, char **tokens) {
	MoveSet moves;
	resetMoveSet(&moves);
	initializeMoveSet(pBoard, &moves);
	
	return true;
}

bool randomMoveCommand(int tokenCount, char **tokens) {
	MoveSet moves;
	resetMoveSet(&moves);
	initializeMoveSetQuiet(pBoard, &moves);
	int moveIndex = randomi(0, moves.totalMoves);
	Move randomMove = moves.moveList[moveIndex];
	printMove(randomMove);
	makeMove(pBoard, randomMove);
	displayBoard(pBoard);
	return true;
}

bool performanceTestCommand(int tokenCount, char **tokens) {
	if(tokenCount != 1) {
		printf("Performance test usage: This command takes one parameter, the depth to be searched.\n");
		return true;
	}
	int depth = atoi(tokens[0]);
	performanceTest(pBoard, depth);
	
	return true;
}

bool helpCommand(int tokenCount, char **tokens) {
	printf("Command list:\n\n\n");
	for(int commandIndex = 0; commandIndex < COMMAND_COUNT; commandIndex++) {
		printf(" %s\n\n", commandDescription[commandIndex]);
	}
	return true;
}

bool moveCommand(int tokenCount, char **tokens) {
	if(tokenCount == 0) {
		printf("You have to specify a move as an additional parameter.\n\n");
		printf("\tmove [PIECE] [ORIGIN] [DESTINATION] [CAPT] [PROM]\n");
		printf("or\n\tmove [CASTLE]\n\n");
		return true;
	}
	Move m;
	if((m = notationToMove(pBoard, tokenCount, tokens))) {
		printMove(m);
		makeMove(pBoard, m);
	}
	return true;
}

bool aboutCommand(int tokenCount, char **tokens) {
	printf("\nThis is a computer chess engine being developed by Conrad and Tom (and Marc).\n%s\n\n", ENGINE_VERSION);
	return true;
}

bool displayCommand(int tokenCount, char **tokens) {
	displayBoard(pBoard);
	return true;
}

bool loadpgnCommand(int tokenCount, char **tokens) {
	if (tokenCount >= 1) loadpgn(tokens[0], pBoard);
	return true;
}

bool debugCommand(int tokenCount, char **tokens) {
	debugBoard(pBoard);
	return true;
}

bool rotateCommand(int tokenCount, char **tokens) {
	rotateBoard(pBoard);
	return true;
}

bool todoCommand(int tokenCount, char **tokens) {
	printf("This is for the developers so they can check what there is to implement.\n");
	return true;
}

bool quitCommand(int tokenCount, char **tokens) {
	return false;
}
