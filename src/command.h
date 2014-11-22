#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

/* Function: type_prompt
 * --------------------
 * type_prompt prints a simple prompt to the screen
 * to let the user know that they have a console
 * interface to the chess engine.
 * The function queries the board to determine whether
 * to display a prompt for white or black.
 */
void type_prompt();

/* Function: tokenize_command
 * -------------------------
 * tokenize_command takes the input string and parses into
 * a token stream kept in a global variable. Internally it
 * uses the string method strtok, which is probably not the best
 * implementation. Consider rewriting this code with strsep or a
 * custom solution.
 */
bool tokenize_command();

/* Function: process_line
 * ---------------------
 * A wrapper function which handles both passing off the input
 * string for tokenization with tokenize_command and which then
 * delegates acting on this input by calling do_command.
 */
bool process_line();

/* Function: read_commands
 * ----------------------
 * read_commands is the entry point for the user input to the engine
 * it accepts characters until a newline is reached, at which point it
 * delegates the string handling to process_line.
 */
void read_commands();

/* Function: do_command
 * -------------------
 * do_command examines the token stream to determine what action
 * to take on the input. If do_command does not recognize the
 * tokens, it prints a message saying so and returns without
 * further execution.
 */
bool do_command();


#endif
