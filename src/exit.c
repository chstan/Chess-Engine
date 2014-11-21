#include <stdlib.h>

#include "exit.h"
#include "log.h"
#include "manage_time.h"
#include "extglobals.h"
#include "zobrist.h"

void prepareForExit() {
    free(pBoard);
    free_hash();
    free_eval_hash();

    // destroy mutexes
    cleanup_logging();
    cleanup_manage_time();
    return;
}
