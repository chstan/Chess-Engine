#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

/* Function: typePrompt
 * --------------------
 * typePrompt prints a simple prompt to the screen
 * to let the user know that they have a console
 * interface to the chess engine.
 * The function queries the board to determine whether
 * to display a prompt for white or black.
 */
void typePrompt();

/* Function: tokenizeCommand
 * -------------------------
 * tokenizeCommand takes the input string and parses into
 * a token stream kept in a global variable. Internally it
 * uses the string method strtok, which is probably not the best
 * implementation. Consider rewriting this code with strsep or a
 * custom solution.
 */
bool tokenizeCommand();

/* Function: processLine
 * ---------------------
 * A wrapper function which handles both passing off the input
 * string for tokenization with tokenizeCommand and which then
 * delegates acting on this input by calling doCommand.
 */
bool processLine();

/* Function: readCommands
 * ----------------------
 * readCommands is the entry point for the user input to the engine
 * it accepts characters until a newline is reached, at which point it
 * delegates the string handling to processLine.
 */
void readCommands();

/* Function: doCommand
 * -------------------
 * doCommand examines the token stream to determine what action
 * to take on the input. If doCommand does not recognize the
 * tokens, it prints a message saying so and returns without
 * further execution.
 */
bool doCommand();


#endif
