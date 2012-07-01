#include <stdlib.h>

#include "exit.h"
#include "extglobals.h"

void prepareForExit() {
	free(pBoard);
	return;
}
