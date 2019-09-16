//Joe Uzdzinski
//config header file

#ifndef RNG_H
#define RNG_H

#include "structs.h"

int SEED, INIT_TIME, FIN_TIME, ARRIVE_MIN, ARRIVE_MAX, QUIT_PROB, CPU_MIN, CPU_MAX, DISK1_MIN, DISK1_MAX, DISK2_MIN, DISK2_MAX;

void configConstants();
void printToLog(EVENT* thisEvent);

#endif