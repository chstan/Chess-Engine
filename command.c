#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "protos.h"
#include "extglobals.h"

void typePrompt() {
	switch(pBoard->info.nextMove) {
		case W:
			printf("WHITE> ");
		break;
		case B:
			printf("BLACK> ");
		break;
		default:
			printf("Board in inconsistent state> ");
		break;
	}
}

bool tokenizeCommand() {
	TOKEN_COUNT = 0;
	char* currentToken;
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
	if(!doCommand()) return false;
	return true;
}

void readCommands() {
	char c;
	typePrompt();
	
	while((c = getc(stdin)) != EOF) {
		if(c == '\n') {
			//handle the typed line and determine whether or not we need to exit
			if(!processLine()) return;
			typePrompt();
		} else {
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
	if(!strcmp(TOKENS[0], "")) {
		return true;
	}
	
	if(!strcmp(TOKENS[0], "help") || !strcmp(TOKENS[0], "h") || !strcmp(TOKENS[0], "?")) {
		printf("Command List:\n\n");
		printf("move [moveParam]\n");
		printf("rotate\n");
		printf("display\n");
		printf("todo\n");
		printf("exit/quit\n");
		printf("help/h/?\n");
		printf("about\n");
		printf("\n");
		return true;
	}
	
	if(!strcmp(TOKENS[0], "move")) {
		if(TOKEN_COUNT == 1) {
			printf("You have to specify a move as an additional parameter.\n\n");
			printf("\tmove [PIECE] [ORIGIN] [DESTINATION] [CAPT] [PROM]\n");
			printf("or\n\tmove [CASTLE]\n\n");
			return true;
		}
		Move m;
		if((m = notationToMove(pBoard)))
			makeMove(pBoard, m);
		printMove(m);
		return true;
	}
	
	if(!strcmp(TOKENS[0], "about")) {
		printf("\nThis is a computer chess engine being developed by Conrad and Marc.\n%s\n\n", ENGINE_VERSION);
		return true;
	}
	
	if(!strcmp(TOKENS[0], "display")) {
		displayBoard(pBoard);
		return true;
	}
	
	if(!strcmp(TOKENS[0], "rotate")) {
		rotateBoard(pBoard);
		return true;
	}
	
	if(!strcmp(TOKENS[0], "todo")) {
		printf("This is for the developers so they can check what there is to implement.\n");
		return true;
	}
	
	if(!strcmp(TOKENS[0], "exit") || !strcmp(TOKENS[0], "quit")) {
		return false;
	}
	
	alert(UNKNOWN_COMMAND);
	return true;
}