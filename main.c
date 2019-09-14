//Joe Uzdzinski
//main source file

#include "config.h"
#include "rng.h"
#include "queue.h"
#include "event.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

int currentTime = 0;
int total = 0;



int main() {
    srand(SEED);

    configConstants();
    
    //allocating memory for start pointers for each queue
    NODE* EVENT_Qptr = malloc(sizeof(NODE));
    EVENT_Qptr -> nextPtr = NULL;

    NODE* CPU_Qptr = malloc(sizeof(NODE));
    CPU_Qptr -> nextPtr = NULL;

    NODE* DISK1_Qptr = malloc(sizeof(NODE));
    DISK1_Qptr -> nextPtr = NULL;

    NODE* DISK2_Qptr = malloc(sizeof(NODE));
    DISK2_Qptr -> nextPtr = NULL;

    //testing for NULL to see if memory was allocated successfully
    if (!EVENT_Qptr || !CPU_Qptr || !DISK1_Qptr || !DISK2_Qptr) {
        printf("No memory allocated.\n");
        return 0;
    }

    //creating instance of component for each component
    //setting each components status to IDLE
    COMPONENT* CPU = malloc(sizeof(COMPONENT));
    CPU -> status = IDLE;
    COMPONENT* DISK1 = malloc(sizeof(COMPONENT));
    DISK1 -> status = IDLE;
    COMPONENT* DISK2 = malloc(sizeof(COMPONENT));
    DISK2 -> status = IDLE;
    
    
    simulationStart(&EVENT_Qptr);


    int running = 1;
    while (running){ // main simulation loop (runs until hit SIMULATION_FINISH event)
        

        EVENT* nextEvent = deq (&EVENT_Qptr); //dequeue next event from event queue
        currentTime = nextEvent -> time; // update currentTime of simulation

        printf("\nCurrent Time: %d\n", currentTime);
        
        printf("-------------------------\n");

        printf("Next Processed Event:\n\n");
        printf("job %d ", nextEvent -> jobNo);
        printEventType(nextEvent -> type);
        printf(" %d \n", nextEvent -> time);

        printf("-------------------------\n");

        printf("CPU status: %d\n", CPU -> status);
        printf("DISK1 status: %d\n", DISK1 -> status);
        printf("DISK2 status: %d\n", DISK2 -> status);

        printf("-------------------------\n");

        printf("Event Q\n\n");
        printq(&EVENT_Qptr);

        printf("-------------------------\n");

        printf("CPU Q\n\n");
        printq(&CPU_Qptr);

        printf("-------------------------\n");

        printf("DISK1 Q\n\n");
        printq(&DISK1_Qptr);

        printf("-------------------------\n");

        printf("DISK2 Q\n\n");
        printq(&DISK2_Qptr);

        printf("_____________________________________________________\n");
        

        switch (nextEvent -> type) { // switch case to determine how to handle event
            
            case PROCESS_ARRIVAL: {
                processArrivalHandler(&EVENT_Qptr, &CPU_Qptr, nextEvent);
                break;
            }
            case CPU_BEGIN: {
                cpuBeginHandler(&EVENT_Qptr, nextEvent);
                break;
            }
            case CPU_FINISH: { 
                cpuFinishHandler(&EVENT_Qptr, nextEvent, &DISK1_Qptr, &DISK2_Qptr, CPU);
                break;
            }
            case PROCESS_EXIT: { 
                processExitHandler(nextEvent);
                total++;
                break;
            }
            case DISK1_ARRIVAL: { 
                disk1ArrivalHandler(&DISK1_Qptr, nextEvent);
                break;
            }
            case DISK2_ARRIVAL: {
                disk1ArrivalHandler(&DISK2_Qptr, nextEvent);
                break;
            }
            case DISK1_BEGIN: { 
                disk1BeginHandler(&EVENT_Qptr, nextEvent);
                break;
            }
            case DISK2_BEGIN: {
                disk2BeginHandler(&EVENT_Qptr, nextEvent);
                break;
            }
            case DISK1_FINISH: { 
                disk1FinishHandler(&CPU_Qptr, nextEvent, DISK1);
                break;
            }
            case DISK2_FINISH: { 
                disk2FinishHandler(&CPU_Qptr, nextEvent, DISK2);
                break;
            }
            case SIMULATION_FINISH: { 
                printf("The simulation has finished.\n");
                printf("%d processes have exited.\n", total);
                running = !running;
                break;
            }
        }
    // update CPU and Disk queues, creates new events where appropriate
    updateCPU(&EVENT_Qptr, &CPU_Qptr, CPU, currentTime); 
    updateDISK1(&EVENT_Qptr, &DISK1_Qptr, DISK1, currentTime);
    updateDISK2(&EVENT_Qptr, &DISK2_Qptr, DISK2, currentTime);

    }
    return 0;
}