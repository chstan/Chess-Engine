#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <stdbool.h>

#define COMMAND_COUNT 16

typedef bool (*CommandCB)(int tokenCount, char **tokens);

bool listMovesCommand(int tokenCount, char **tokens);
bool randomMoveCommand(int tokenCount, char **tokens);
bool moveCommand(int tokenCount, char **tokens);
bool unmoveCommand(int tokenCount, char **tokens);

bool performanceTestCommand(int tokenCount, char **tokens);
bool divideCommand(int tokenCount, char **tokens);

bool helpCommand(int tokenCount, char **tokens);
bool aboutCommand(int tokenCount, char **tokens);

bool loadpgnCommand(int tokenCount, char **tokens);
bool loadFENCommand(int tokenCount, char **tokens);

bool debugCommand(int tokenCount, char **tokens);
bool debugNotationCommand(int tokenCount, char **tokens);

bool displayCommand(int tokenCount, char **tokens);
bool rotateCommand(int tokenCount, char **tokens);

bool todoCommand(int tokenCount, char **tokens);
bool quitCommand(int tokenCount, char **tokens);

#endif
