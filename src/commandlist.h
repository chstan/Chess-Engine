#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <stdbool.h>

#define COMMAND_COUNT 17

typedef bool (*CommandCB)(int token_count, char **tokens);

bool list_moves_command(int token_count, char **tokens);
bool random_move_command(int token_count, char **tokens);
bool move_command(int token_count, char **tokens);
bool unmove_command(int token_count, char **tokens);

bool play_command(int token_count, char **tokens);

bool performance_test_command(int token_count, char **tokens);
bool divide_command(int token_count, char **tokens);

bool help_command(int token_count, char **tokens);
bool about_command(int token_count, char **tokens);

bool load_pgn_command(int token_count, char **tokens);
bool load_FEN_command(int token_count, char **tokens);

bool debug_command(int token_count, char **tokens);
bool debug_notation_command(int token_count, char **tokens);

bool display_command(int token_count, char **tokens);
bool rotate_command(int token_count, char **tokens);

bool todo_command(int token_count, char **tokens);
bool quit_command(int token_count, char **tokens);

#endif
