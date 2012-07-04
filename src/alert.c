#include <stdio.h>

#include "defines.h"
#include "alert.h"

void alert(int ALERT) {
	switch(ALERT) {
		case UNKNOWN_COMMAND:
			printf("Alert: Could not find any command matching the one typed.\n");
		break;
		default:
			// CATCHES OTHERS
		break;
	}
}