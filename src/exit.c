#include <stdlib.h>

#include "exit.h"
#include "extglobals.h"
#include "zobrist.h"

void prepareForExit() {
    free(pBoard);
    free_hash();
    return;
}
