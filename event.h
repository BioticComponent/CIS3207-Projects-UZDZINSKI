//Joe Uzdinski
//event header file

#ifndef EVENT_H
#define EVENT_H


#include "structs.h"


EVENT* createEvent (eventType type, int jobNo, int time);
void simulationStart (NODE* *startPtr);
void processArrivalHandler (NODE* *EVENT_Qptr, NODE* *CPU_Qptr, EVENT* deqEvent);
void updateCPU (NODE* *EVENT_Qptr, NODE* *CPU_Qptr, COMPONENT* CPU, int currentTime);
void updateDISK1 (NODE* *EVENT_Qptr, NODE* *DISK1_Qptr, COMPONENT* DISK1, int currentTime);
void updateDISK2 (NODE* *EVENT_Qptr, NODE* *DISK2_Qptr, COMPONENT* DISK2, int currentTime);
int cpuBeginHandler(NODE* *EVENT_Qptr, EVENT* cpuEvent);
void cpuFinishHandler(NODE* *EVENT_Qptr, EVENT* thisEvent, NODE* *DISK1_Qptr, NODE* *DISK2_Qptr, COMPONENT* CPU);
void processExitHandler(EVENT* exitEvent);
void disk1ArrivalHandler(NODE* *DISK1_Qptr, EVENT* thisEvent);
void disk2ArrivalHandler(NODE* *DISK2_Qptr, EVENT* thisEvent);
int disk1BeginHandler(NODE* *EVENT_Qptr, EVENT* disk1Event);
int disk2BeginHandler(NODE* *EVENT_Qptr, EVENT* disk2Event);
void disk1FinishHandler(NODE* *CPU_Qptr, EVENT* thisEvent, COMPONENT* DISK1);
void disk2FinishHandler(NODE* *CPU_Qptr, EVENT* thisEvent, COMPONENT* DISK2);
void printEventType (eventType type);

#endif