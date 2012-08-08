#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "command.h"
#include "commandlist.h"
#include "defines.h"
#include "extglobals.h"
#include "alert.h"
#include "move/movegen.h"
#include "move/notation.h"
#include "aux/random.h"
#include "test/movegentest.h"

extern CommandCB commandCallbacks[];

extern char *commandTriggers[];

extern char *commandDescription[];

void typePrompt() {
	assert(pBoard->info.toPlay == W || pBoard->info.toPlay == B);
	switch(pBoard->info.toPlay) {
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
	
	for(int commandIndex = 0; commandIndex < COMMAND_COUNT; commandIndex++) {
		if(!strcmp(TOKENS[0], commandTriggers[commandIndex])) {
			return (*commandCallbacks[commandIndex])(TOKEN_COUNT-1, TOKENS+1);
		}
	}
	
	alert(UNKNOWN_COMMAND);
	return true;
}
