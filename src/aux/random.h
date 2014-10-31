#ifndef RANDOM_H
#define RANDOM_H

/** Function: initRand
 *  ------------------
 * initRand ensures that the random number generator
 * is seeded. It expects no arguments, and internally uses
 * the time to seed the c random functions.
**/
void initRandom();

/** Function: randomi
 *  ----------
 * returns a random integer.
 *
 * Variants ?:
 *  - random(int)
 *      - returns and integer between 0 and int (or between int and 0 if negative)
 *
 *  - random(void)
 *      - returns 0 or 1 randomly with equal probability
 *
 *  - random(int, int)
 *      - returns an integer in the range defined by the parameters
**/
int randomi(const int lowerLimit, const int upperLimit);

/** Function: randomf
 *  ---------------
 * returns a random float.
**/
float randomf();


#endif
