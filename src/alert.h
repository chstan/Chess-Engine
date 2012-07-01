#ifndef ALERT_H
#define ALERT_H 

/* Function: alert
 * ---------------
 * alert is a general interface which (should be) is
 * used through the engine as a wrapper for a printf.
 * it helps avoid cluttered code for common messages to
 * the user.
 */
void alert(int ALERT);

#endif
