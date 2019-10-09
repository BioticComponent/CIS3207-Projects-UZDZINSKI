//Joe Uzdzinski
//event source file
//includes all functions pertaining to an event and its handling

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
    tempPtr -> cpuQresponse = 0;
    tempPtr -> disk1Qresponse = 0;
    tempPtr -> disk2Qresponse = 0;
    return tempPtr;
}

//STARTS A NEW SIMULATION
//add in first job (PROCESS_ARRIVAL)
//add in last job (SIM_FINISH)
void simulationStart (NODE* *EVENT_QPtr) {
    //creating first event (job 1)
    EVENT* firstEvent = createEvent (PROCESS_ARRIVAL, 1, INIT_TIME);
    //adding job 1 to event queue
    enq(EVENT_QPtr, firstEvent, PRIORITY);
    
    //creating simulation end event (last job)
    EVENT* lastEvent = createEvent (SIMULATION_FINISH, -1, FIN_TIME);
    //adding last job to event queue
    enq(EVENT_QPtr, lastEvent, PRIORITY);
    
}

//everytime a process arrives, it gets handled here
//new event is created and put into the event q
void processArrivalHandler (NODE* *EVENT_Qptr, NODE* *CPU_Qptr, EVENT* deqEvent) {
    deqEvent -> cpuQresponse = deqEvent -> time;
    //adding dequeued event to the CPU queue
    enq (CPU_Qptr, deqEvent, FIFO);
    
    //getting random number for next event arrival time
    int randNum = randomNumberBetween (ARRIVE_MIN, ARRIVE_MAX);
    int nextEventsTime = deqEvent -> time + randNum;
  
    //creating next event arrival and adding it to EVENT queue
    EVENT* newEvent = createEvent (PROCESS_ARRIVAL, deqEvent -> jobNo + 1, nextEventsTime);
    enq (EVENT_Qptr, newEvent, PRIORITY);
}

//determines finish time of event at cpu, and changes event type to CPU_FINISH
int cpuBeginHandler(NODE* *EVENT_Qptr, EVENT* cpuEvent) {
    //getting random number for cpu servicing time
    int randNum = randomNumberBetween (CPU_MIN, CPU_MAX);
    
    //setting time and type for new cpu finish event
    cpuEvent -> time = cpuEvent -> time + randNum;
    cpuEvent -> type = CPU_FINISH;

    
    //adding it back into the event queue
    enq (EVENT_Qptr, cpuEvent, PRIORITY);

    return randNum;
}

//when a process reaches here, it either exits the system, or stays in and goes to a disk for further processsing
void cpuFinishHandler(NODE* *EVENT_Qptr, EVENT* thisEvent, NODE* *DISK1_Qptr, NODE* *DISK2_Qptr, COMPONENT* CPU) {
    int randNum = randomNumberBetween (1, 10);
    //gives a 20% chance for the process to be completed and leave the system
    if (randNum <= QUIT_PROB) {
        thisEvent -> type = PROCESS_EXIT;
    } else {
        //if process stays in simulation, enters shorter disk queue
        //if equal randomly chooses a disk
        int diskQrandNum = randomNumberBetween (1,10);
        if (queueLength(DISK1_Qptr) == queueLength(DISK2_Qptr)) {
            if (diskQrandNum <= 5) {
                thisEvent -> disk1Qresponse = thisEvent -> time;
                thisEvent -> type = DISK1_ARRIVAL;
            } else {
                thisEvent -> disk2Qresponse = thisEvent -> time;
                thisEvent -> type = DISK2_ARRIVAL;
            } 
        } else if (queueLength(DISK1_Qptr) < queueLength(DISK2_Qptr)) {
            thisEvent -> disk1Qresponse = thisEvent -> time;
            thisEvent -> type = DISK1_ARRIVAL;
        } else {
            thisEvent -> disk2Qresponse = thisEvent -> time;
            thisEvent -> type = DISK2_ARRIVAL;
        }
    }
    //setting CPU status to idle 
    CPU -> status = IDLE;
    //put event back in the queue 
    enq (EVENT_Qptr, thisEvent, PRIORITY);
}

//this event will no longer see the light of day
void processExitHandler(EVENT* exitEvent) {
    //frees memory of the finished process
    free(exitEvent);
}

//this handler simply places the event into the disk1 q
void disk1ArrivalHandler(NODE* *DISK1_Qptr, EVENT* thisEvent) {
    //add the event to the disk1 queue
    enq (DISK1_Qptr, thisEvent, FIFO);
}

//this handler simply places the event into the disk1 q
void disk2ArrivalHandler(NODE* *DISK2_Qptr, EVENT* thisEvent) {
    //add the event to the disk2 queue
    enq (DISK2_Qptr, thisEvent, FIFO);
}

//determines finish time of event at disk1, and changes event type to DISK1_FINISH
int disk1BeginHandler(NODE* *EVENT_Qptr, EVENT* disk1Event) {
    //getting random number for disk1 servicing time
    int randNum = randomNumberBetween (DISK1_MIN, DISK1_MAX);
    
    //setting time and type for new disk1 finish event
    disk1Event -> time = disk1Event -> time + randNum;
    disk1Event -> type = DISK1_FINISH;
    
    //adding it back into the event queue
    enq (EVENT_Qptr, disk1Event, PRIORITY);

    return randNum;
}
//determines finish time of event at disk2, and changes event type to DISK2_FINISH
int disk2BeginHandler(NODE* *EVENT_Qptr, EVENT* disk2Event) {
    //getting random number for disk2 servicing time
    int randNum = randomNumberBetween (DISK2_MIN, DISK2_MAX);
    
    //setting time and type for new disk2 finish event
    disk2Event -> time = disk2Event -> time + randNum;
    disk2Event -> type = DISK2_FINISH;
    
    //adding it back into the event queue
    enq (EVENT_Qptr, disk2Event, PRIORITY);

    return randNum;
}

//sets the disk to idle and sends the event to the cpu queue
void disk1FinishHandler(NODE* *CPU_Qptr, EVENT* thisEvent, COMPONENT* DISK1) {
    //setting disk1 status to idle 
    DISK1 -> status = IDLE;

    thisEvent -> cpuQresponse = thisEvent -> time;
    //put event into CPU queue for further processing
    enq (CPU_Qptr, thisEvent, FIFO);
}

//sets the disk to idle and sends the event to the cpu queue
void disk2FinishHandler(NODE* *CPU_Qptr, EVENT* thisEvent, COMPONENT* DISK2) {
    //setting disk1 status to idle 
    DISK2 -> status = IDLE;

    thisEvent -> cpuQresponse = thisEvent -> time;
    //put event into CPU queue for further processing
    enq (CPU_Qptr, thisEvent, FIFO);
}

//updates the cpu
//calculate response time (EXPLAINED IN README)
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
        //subtracting the current time from first reponse time to find new time
        cpuStartEvent -> cpuQresponse -= currentTime;
        //multiplied by -2 and added to itself to make positve
        cpuStartEvent -> cpuQresponse += (-2*(cpuStartEvent -> cpuQresponse));
        //add the event back to the event queue
        enq(EVENT_Qptr, cpuStartEvent, PRIORITY);
    }
}

//updates disk1
//calculate response time (EXPLAINED IN README)
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
        //subtracting the current time from first reponse time to find new time
        disk1StartEvent -> disk1Qresponse -= currentTime;
        //multiplied by -2 and added to itself to make positve
        disk1StartEvent -> disk1Qresponse += (-2*(disk1StartEvent -> disk1Qresponse));
        //add the event back to the event queue
        enq(EVENT_Qptr, disk1StartEvent, PRIORITY);
    }
}

//updates disk2
//calculate response time (EXPLAINED IN README)
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
        //subtracting the current time from first reponse time to find new time
        disk2StartEvent -> disk2Qresponse -= currentTime;
        //multiplied by -2 and added to itself to make positve
        disk2StartEvent -> disk2Qresponse += (-2*(disk2StartEvent -> disk2Qresponse));
        //add the event back to the event queue
        enq(EVENT_Qptr, disk2StartEvent, PRIORITY);
    }
}

//used for debugging, not implemented in actual simulation
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
