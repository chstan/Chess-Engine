#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "protos.h"
#include "extglobals.h"

void typePrompt() {
	printf("eng> ");
}

bool processLine() {
	// process all the commands currently sitting in CMD_BUFFER
	CMD_BUFFER[CMD_BUFFER_COUNT] = '\0';
	while(CMD_BUFFER_COUNT) {
		if(!do_command(CMD_BUFFER)) return false;
		type_prompt();
	}
	return true;
}

void readCommands() {
	char c;
	typePrompt();
	
	while((c = getc(stdin)) != EOF) {
		if(c == '\n') {
			//handle the typed line and determine whether or not we need to exit
			if(!processLine()) return;
		} else {
			if(CMD_BUFFER_COUNT >= MAX_CMD_BUFFER-1) {
				printf("Input refused: input buffer full.\n");
				CMD_BUFFER_COUNT = 0;
			}
			CMD_BUFFER[CMD_BUFFER_COUNT++] = c;
		}
	}
}

bool doCommand(const char *cmd) {
	if(!strcmp(cmd, "")) {
		CMD_BUFFER_COUNT = '\0';
		return true;
	}
	
	if(!strcmp(cmd, "help") || !strcmp(cmd, "h") || !strcmp(cmd, "?")) {
		printf("This is the help command.\n");
		CMD_BUFFER_COUNT = '\0';
		return true;
	}
	
	if(!strcmp(cmd, "todo")) {
		printf("This is for the developers so they can check what there is to implement.\n");
		CMD_BUFFER_COUNT = '\0';
		return true;
	}
	
	if(!strcmp(cmd, "exit") || !strcmp(cmd, "quit")) {
		CMD_BUFFER_COUNT = '\0';
		return false;
	}
	
	alert(UNKNOWN_COMMAND);
	CMD_BUFFER_COUNT = '\0';
	return true;
}