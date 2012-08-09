#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <stdbool.h>

#define COMMAND_COUNT 11

typedef bool (*CommandCB)(const int tokenCount, const char **tokens);

bool listMovesCommand(const int tokenCount, const char **tokens);
bool randomMoveCommand(const int tokenCount, const char **tokens);
bool performanceTestCommand(const int tokenCount, const char **tokens);
bool helpCommand(const int tokenCount, const char **tokens);
bool moveCommand(const int tokenCount, const char **tokens);
bool aboutCommand(const int tokenCount, const char **tokens);
bool displayCommand(const int tokenCount, const char **tokens);
bool debugCommand(const int tokenCount, const char **tokens);
bool rotateCommand(const int tokenCount, const char **tokens);
bool todoCommand(const int tokenCount, const char **tokens);
bool quitCommand(const int tokenCount, const char **tokens);

#endif
