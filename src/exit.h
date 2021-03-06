#ifndef EXIT_H
#define EXIT_H

/* Function: prepare_for_exit
 * ------------------------
 * Frees all dynamically allocated memory.
 * At the moment, the only memory there is to free
 * is the board. Since chess engines are a complex
 * breed of beast, any logic we might want to add at exit
 * can be put here as well.
 */
void prepare_for_exit();

#endif
