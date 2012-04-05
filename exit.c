#include "protos.h"
#include "extglobals.h"
#include <stdlib.h>

void prepareForExit() {
	free(pBoard);
	return;
}