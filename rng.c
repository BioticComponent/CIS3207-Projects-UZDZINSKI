//Joe Uzdzinski
//rng source file

#include "rng.h"
#include <stdlib.h>

//rng function that returns a random number in between the interval of upper and lower
int rng (int lower, int upper) {

    return (rand() % (upper - lower + 1)) + lower;
}