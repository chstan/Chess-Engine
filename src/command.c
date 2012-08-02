#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "command.h"
#include "defines.h"
#include "extglobals.h"
#include "alert.h"
#include "move/movegen.h"
#include "move/notation.h"
#include "aux/random.h"
#include "test/movegentest.h"

void typePrompt() {
	assert(pBoard->info.nextMove == W || pBoard->info.nextMove == B);
	switch(pBoard->info.nextMove) {
		case W:
			printf("WHITE> ");
		break;
		case B:
			printf("BLACK> ");
		break;
	}
}

bool tokenizeCommand() {
	TOKEN_COUNT = 0;
	char* currentToken;
	// Don't try to tokenize an empty buffer. Trying to call
	// strtok on the buffer without doing so would cause a segfault.
	if (!strcmp(CMD_BUFFER, "")) return false;
	for(currentToken = strtok(CMD_BUFFER, " ");
			currentToken && TOKEN_COUNT < MAX_TOKENS; currentToken = strtok(NULL, " ")) {
		TOKENS[TOKEN_COUNT] = currentToken;
		TOKEN_COUNT++;
	}
	return true;
}

bool processLine() {
	// process all the commands currently sitting in CMD_BUFFER
	CMD_BUFFER[CMD_BUFFER_COUNT] = '\0';
	if (!tokenizeCommand()) return true;
	CMD_BUFFER_COUNT = 0;
	// Send a message to exit if doCommand read a quit signal
	if(!doCommand()) return false;
	return true;
}

void readCommands() {
	char c;
	typePrompt();
	
	// read until a newline or we have filled the buffer.
	while((c = getc(stdin)) != EOF) {
		if(c == '\n') {
			// handle the typed line and determine whether or not we need to exit
			if(!processLine()) return;
			typePrompt();
		} else {
			// refuse any characters that would overfill the buffer
			if(CMD_BUFFER_COUNT >= MAX_CMD_BUFFER-1) {
				printf("Input refused: input buffer full.\n");
				CMD_BUFFER_COUNT = 0;
			}
			CMD_BUFFER[CMD_BUFFER_COUNT++] = c;
		}
	}
	return;
}

bool doCommand() {
	
	// Blank token streams should be rejected
	if(!strcmp(TOKENS[0], "")) {
		return true;
	}
	
	// Print all possible moves from the current position
	if(!strcmp(TOKENS[0], "list-moves")) {
		MoveSet moves;
		resetMoveSet(&moves);
		initializeMoveSet(pBoard, &moves);
		
		return true;
	}
	
	if(!strcmp(TOKENS[0], "random-move")) {
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
	
	if(!strcmp(TOKENS[0], "performance-test")) {
		int depth = atoi(TOKENS[1]);
		performanceTest(pBoard, depth);
		
		return true;
	}
	
	// List help information
	if(!strcmp(TOKENS[0], "help") || !strcmp(TOKENS[0], "h") || !strcmp(TOKENS[0], "?")) {
		printf("Command List:\n\n");
		printf("move [moveParam]\n");
		printf("list-moves\n");
		printf("random-move");
		printf("performance-test");
		printf("debug-board\n");
		printf("rotate\n");
		printf("display\n");
		printf("todo\n");
		printf("exit/quit\n");
		printf("help/h/?\n");
		printf("about\n");
		printf("\n");
		return true;
	}
	
	// Make a move, if improperly formatted tokens, print
	// helpful advice on how to use the command instead
	if(!strcmp(TOKENS[0], "move")) {
		if(TOKEN_COUNT == 1) {
			printf("You have to specify a move as an additional parameter.\n\n");
			printf("\tmove [PIECE] [ORIGIN] [DESTINATION] [CAPT] [PROM]\n");
			printf("or\n\tmove [CASTLE]\n\n");
			return true;
		}
		Move m;
		if((m = notationToMove(pBoard))) {
			printMove(m);
			makeMove(pBoard, m);
		}
		return true;
	}
	
	// Information about the engine
	if(!strcmp(TOKENS[0], "about")) {
		printf("\nThis is a computer chess engine being developed by Conrad and Marc.\n%s\n\n", ENGINE_VERSION);
		return true;
	}
	
	// Show the board
	// TODO: Add flags to show additional information about the
	// 			 internal board structure, material etc.
	if(!strcmp(TOKENS[0], "display")) {
		displayBoard(pBoard);
		return true;
	}

	// run debugBoard
	if(!strcmp(TOKENS[0], "debug-board")) {
		debugBoard(pBoard);
		return true;
	}
	
	// Rotate the board,
	// currently, the display function does not support rotating
	// to override displaying the pieces of the next player to move
	// that can be implemented easily
	if(!strcmp(TOKENS[0], "rotate")) {
		rotateBoard(pBoard);
		return true;
	}
	
	// In case you're stuck without the git logs
	// or the todo file.
	if(!strcmp(TOKENS[0], "todo")) {
		printf("This is for the developers so they can check what there is to implement.\n");
		return true;
	}
	
	// Quit the program cleanly, drops back into main
	// so that the program can free dynamically allocated memory.
	if(!strcmp(TOKENS[0], "exit") || !strcmp(TOKENS[0], "quit")) {
		return false;
	}
	
	// Alert the user to the fact that no
	// command matching the typed one exists
	alert(UNKNOWN_COMMAND);
	return true;
}
