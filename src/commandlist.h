#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <stdbool.h>

#define COMMAND_COUNT 11

typedef bool (*CommandCB)(const int tokenCount, const char **tokens);

bool listMovesCommand(int tokenCount, const char **tokens);
bool randomMoveCommand(int tokenCount, const char **tokens);
bool performanceTestCommand(int tokenCount, const char **tokens);
bool helpCommand(int tokenCount, const char **tokens);
bool moveCommand(int tokenCount, const char **tokens);
bool aboutCommand(int tokenCount, const char **tokens);
bool displayCommand(int tokenCount, const char **tokens);
bool debugCommand(int tokenCount, const char **tokens);
bool rotateCommand(int tokenCount, const char **tokens);
bool todoCommand(int tokenCount, const char **tokens);
bool quitCommand(int tokenCount, const char **tokens);

#endif
