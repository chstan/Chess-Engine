#include <stdio.h>
#include "defines.h"
#include "protos.h"
#include "globals.h"

/* Function: main
 * --------------
 * entry point for the entire application
 * drops into a loop at readCommands()
 * which exits as soon as the user decides to quit
 * or some error is encountered.
 * 
 * Before quitting, main frees memory with a call
 * to prepare for exit.
 */
int main (int argc, char const *argv[]) {
	printf("%s\n",ENGINE_VERSION);
	
	init();
	
	readCommands();
	
	prepareForExit();
	return 0;
}
