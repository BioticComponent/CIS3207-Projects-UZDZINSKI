//Joe Uzdzinski
//event source file

#include "config.h"
#include "rng.h"
#include "event.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>




//CREATES A NEW EVENT
//takes an int type and time for a new event struct
//returns a pointer to the event
EVENT* createEvent (eventType type, int jobNo, int time) {
    EVENT* tempPtr = (EVENT*)malloc(sizeof(EVENT));
    tempPtr -> type = type;
    tempPtr -> jobNo = jobNo;
    tempPtr -> time = time;
    return tempPtr;
}

void simulationStart (NODE* *startPtr) {
    //creating first event (job 1)
    EVENT* firstEvent = createEvent (PROCESS_ARRIVAL, 1, INIT_TIME);
    //adding job 1 to event queue
    enq(startPtr, firstEvent, PRIORITY);
    
    //creating simulation end event (last job)
    EVENT* lastEvent = createEvent (SIMULATION_FINISH, -1, FIN_TIME);
    //adding last job to event queue
    enq(startPtr, lastEvent, PRIORITY);
    
}

void processArrivalHandler (NODE* *EVENT_Qptr, NODE* *CPU_Qptr, EVENT* deqEvent) {
    //adding dequeued event to the CPU queue
    enq (CPU_Qptr, deqEvent, FIFO);

    //getting random number for next event arrival time
    int randNum = randomNumberBetween (ARRIVE_MIN, ARRIVE_MAX);
    int nextEventsTime = deqEvent -> time + randNum;

    //creating next event arrival and adding it to EVENT queue
    EVENT* newEvent = createEvent (PROCESS_ARRIVAL, deqEvent -> jobNo + 1, nextEventsTime);
    enq (EVENT_Qptr, newEvent, PRIORITY);
    
    // printf("FLAG\n");
    // printq(EVENT_Qptr);
}

void cpuBeginHandler(NODE* *EVENT_Qptr, EVENT* cpuEvent) {
    //getting random number for cpu servicing time
    int randNum = randomNumberBetween (CPU_MIN, CPU_MAX);
    
    //setting time and type for new cpu finish event
    cpuEvent -> time = cpuEvent -> time + randNum;
    cpuEvent -> type = CPU_FINISH;
    
    //adding it back into the event queue
    enq (EVENT_Qptr, cpuEvent, PRIORITY);
}

void cpuFinishHandler(NODE* *EVENT_Qptr, EVENT* thisEvent, NODE* *DISK1_Qptr, NODE* *DISK2_Qptr, COMPONENT* CPU) {
    int randNum = randomNumberBetween (1, 10);
    //gives a 20% chance for the process to be completed and leave the system
    if (randNum <= QUIT_PROB) {
        thisEvent -> type = PROCESS_EXIT;
    } else {
        //if process stays in simulation, enters shorter disk queue
        if (queueLength(DISK1_Qptr) <= queueLength(DISK2_Qptr)) {
            thisEvent -> type = DISK1_ARRIVAL;
        } else {
            thisEvent -> type = DISK2_ARRIVAL;
        }
    }
    //setting CPU status to idle 
    CPU -> status = IDLE;
    //put event back in the queue 
    enq (EVENT_Qptr, thisEvent, PRIORITY);
}

void processExitHandler(EVENT* exitEvent) {
    //frees memory of the finished process
    free(exitEvent);
}

void disk1ArrivalHandler(NODE* *DISK1_Qptr, EVENT* thisEvent) {
    //add the event to the disk1 queue
    enq (DISK1_Qptr, thisEvent, FIFO);
}

void disk2ArrivalHandler(NODE* *DISK2_Qptr, EVENT* thisEvent) {
    //add the event to the disk2 queue
    enq (DISK2_Qptr, thisEvent, FIFO);
}

void disk1BeginHandler(NODE* *EVENT_Qptr, EVENT* disk1Event) {
    //getting random number for disk1 servicing time
    int randNum = randomNumberBetween (DISK1_MIN, DISK1_MAX);
    
    //setting time and type for new disk1 finish event
    disk1Event -> time = disk1Event -> time + randNum;
    disk1Event -> type = DISK1_FINISH;
    
    //adding it back into the event queue
    enq (EVENT_Qptr, disk1Event, PRIORITY);
}

void disk2BeginHandler(NODE* *EVENT_Qptr, EVENT* disk2Event) {
    //getting random number for disk2 servicing time
    int randNum = randomNumberBetween (DISK2_MIN, DISK2_MAX);
    
    //setting time and type for new disk2 finish event
    disk2Event -> time = disk2Event -> time + randNum;
    disk2Event -> type = DISK2_FINISH;
    
    //adding it back into the event queue
    enq (EVENT_Qptr, disk2Event, PRIORITY);
}

void disk1FinishHandler(NODE* *CPU_Qptr, EVENT* thisEvent, COMPONENT* DISK1) {
    //setting disk1 status to idle 
    DISK1 -> status = IDLE;
    //put event into CPU queue for further processing
    enq (CPU_Qptr, thisEvent, FIFO);
}

void disk2FinishHandler(NODE* *CPU_Qptr, EVENT* thisEvent, COMPONENT* DISK2) {
    //setting disk1 status to idle 
    DISK2 -> status = IDLE;
    //put event into CPU queue for further processing
    enq (CPU_Qptr, thisEvent, FIFO);
}

void updateCPU (NODE* *EVENT_Qptr, NODE* *CPU_Qptr, COMPONENT* CPU, int currentTime) {
    //if no job is currently being serviced by CPU and there is an event in the cpu queue
    // then start processing next event
    if ((CPU -> status == IDLE) && (queueLength(CPU_Qptr) > 0)) {
        //set cpu status to busy
        CPU -> status = BUSY;
        //dequeue event from cpu queue
        EVENT* cpuStartEvent = deq (CPU_Qptr);
        //set the events type to CPU_ENTER
        cpuStartEvent -> type = CPU_BEGIN;
        cpuStartEvent -> time = currentTime;
        //add the event back to the event queue
        enq(EVENT_Qptr, cpuStartEvent, PRIORITY);
    }
}

void updateDISK1 (NODE* *EVENT_Qptr, NODE* *DISK1_Qptr, COMPONENT* DISK1, int currentTime) {
    //if no job is currently being serviced by DISK1 and there is an event in the DISK1 queue
    // then start processing next event
    if ((DISK1 -> status == IDLE) && (queueLength(DISK1_Qptr) > 0)) {
        //set DISK1 status to busy
        DISK1 -> status = BUSY;
        //dequeue event from cpu queue
        EVENT* disk1StartEvent = deq (DISK1_Qptr);
        //set the events type to CPU_ENTER
        disk1StartEvent -> type = DISK1_BEGIN;
        disk1StartEvent -> time = currentTime;
        //add the event back to the event queue
        enq(EVENT_Qptr, disk1StartEvent, PRIORITY);
    }
}

void updateDISK2 (NODE* *EVENT_Qptr, NODE* *DISK2_Qptr, COMPONENT* DISK2, int currentTime) {
    //if no job is currently being serviced by DISK1 and there is an event in the DISK1 queue
    // then start processing next event
    if ((DISK2 -> status == IDLE) && (queueLength(DISK2_Qptr) > 0)) {
        //set DISK1 status to busy
        DISK2 -> status = BUSY;
        //dequeue event from cpu queue
        EVENT* disk2StartEvent = deq (DISK2_Qptr);
        //set the events type to CPU_ENTER
        disk2StartEvent -> type = DISK2_BEGIN;
        disk2StartEvent -> time = currentTime;
        //add the event back to the event queue
        enq(EVENT_Qptr, disk2StartEvent, PRIORITY);
    }
}

void printEventType (eventType type) {
    
    switch (type) {
            
            case PROCESS_ARRIVAL: {
                printf("PROCESS_ARRIVAL");
                break;
            }
            case CPU_BEGIN: {
                printf("CPU_BEGIN");
                break;
            }
            case CPU_FINISH: { 
                printf("CPU_FINISH"); 
                break;
            }
            case PROCESS_EXIT: { 
                printf("PROCESS_EXIT");
                break;
            }
            case DISK1_ARRIVAL: { 
                 printf("DISK1_ARRIVAL");               
                break;
            }
            case DISK1_BEGIN: { 
                 printf("DISK1_BEGIN");               
                break;
            }
            case DISK2_ARRIVAL: {
                printf("DISK2_ARRIVAL");                
                break;
            }
            case DISK2_BEGIN: {
                printf("DISK2_BEGIN");                
                break;
            }
            case DISK1_FINISH: { 
                printf("DISK1_FINISH");
                break;
            }
            case DISK2_FINISH: { 
                printf("DISK2_FINISH");
                break;
            }
            case SIMULATION_FINISH: { 
                printf("SIMULATION_FINISH");
                break;
            }
    }
}
