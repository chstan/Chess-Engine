#include <stdio.h>
#include "defines.h"
#include "protos.h"
#include "globals.h"

int main (int argc, char const *argv[])
{
	printf(ENGINE_VERSION);
	printf("\n");
	
	init();
	
	read_commands();
	return 0;
}