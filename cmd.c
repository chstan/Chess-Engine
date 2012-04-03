#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "protos.h"
#include "extglobals.h"

void type_prompt() {
	printf("eng> ");
}

void process_line() {
	// process all the commands currently sitting in CMD_BUFFER
	CMD_BUFFER[CMD_BUFFER_COUNT] = '\0';
	while(CMD_BUFFER_COUNT) {
		if(!do_command(CMD_BUFFER)) return;
		type_prompt();
	}
}

void read_commands() {
	char c;
	type_prompt();
	
	while((c = getc(stdin)) != EOF) {
		if(c == '\n') {
			process_line();
		} else {
			if(CMD_BUFFER_COUNT >= MAX_CMD_BUFFER-1) {
				printf("Input refused: input buffer full.\n");
				CMD_BUFFER_COUNT = 0;
			}
			CMD_BUFFER[CMD_BUFFER_COUNT++] = c;
		}
	}
}

bool do_command(const char *cmd) {
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