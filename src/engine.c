#include <stdio.h>
#include <string.h>

#include "engine.h"
#include "uci.h"
#include "log.h"
#include "defines.h"
#include "globals.h"
#include "init.h"
#include "exit.h"
#include "command.h"

void usage_message() {
    printf("If called with parameters two are required.\n");
    printf("The first should be --uci to signal use with a GUI\n");
    printf("And the second should be the path to a file to use for logging.\n");
}

/* Function: main
 * --------------
 * entry point for the entire application
 * drops into a loop at read_commands()
 * which exits as soon as the user decides to quit
 * or some error is encountered.
 *
 * Before quitting, main frees memory with a call
 * to prepare for exit.
 */
int main (int argc, char const *argv[]) {
    if (argc == 1) {
        set_logging_location("engine.log");
        pre_init();
        init();

        printf("%s\n",ENGINE_VERSION);

        read_commands();
        prepare_for_exit();
    } else {
        if (argc == 3 && !strcmp(argv[1], "--uci")) {
            // need a logging location as the second argument
            // check if a UCI board wants to use the engine
            set_logging_location(argv[2]);
            uci_loop();
        } else {
            // otherwise exit
            usage_message();
        }
    }

    return 0;
}
