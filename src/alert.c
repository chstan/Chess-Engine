#include "defines.h"
#include "stdio.h"

/* Function: alert
 * ---------------
 * alert is a general interface which (should be) is
 * used through the engine as a wrapper for a printf.
 * it helps avoid cluttered code for common messages to
 * the user.
 */
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
