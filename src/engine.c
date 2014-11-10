#include <stdio.h>
#include <string.h>

#include "uci.h"
#include "engine.h"
#include "defines.h"
#include "globals.h"
#include "init.h"
#include "exit.h"
#include "command.h"

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
    if (argc == 1) {
        init();

        printf("%s\n",ENGINE_VERSION);

        readCommands();
        prepareForExit();
    } else {
        if (argc == 2 && !strcmp(argv[1], "--uci")) {
            // check if a UCI board wants to use the engine
            uci_loop();
        }
        // otherwise exit
    }

    return 0;
}
