#include <stdio.h>
#include "defines.h"
#include "protos.h"
#include "globals.h"

int main (int argc, char const *argv[])
{
	printf("%s\n",ENGINE_VERSION);
	
	init();
	
	readCommands();
	
	exit();
	return 0;
}