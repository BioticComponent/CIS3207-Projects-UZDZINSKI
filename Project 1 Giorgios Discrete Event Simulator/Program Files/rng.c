//Joe Uzdzinski
//rng source file

#include <stdlib.h>
#include "rng.h"
#include "config.h"



//rng function that returns a random number in between the interval of upper and lower
int randomNumberBetween (int lower, int upper) {
    
    return (rand() % (upper - lower + 1)) + lower;
}