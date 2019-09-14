///////////////////////////////
///FOR TESTING AND DEBUGGING///
///////////////////////////////

#include "rng.h"
#include "queue.h"
#include "event.h"
#include <stdio.h> 
#include <stdlib.h>

int main() {
    srand(0);
    int count = 0;
    while (count < 100) {
        printf("%d\n", rng(50, 60));
    }
    return 0;

    //allocating memory for startPtr
    //startPtr is the first Ptr of the queue
    NODE* startPtr = malloc(sizeof(NODE));
    startPtr -> nextPtr = NULL;

    //testing for NULL to see if memory was allocated successfully
    if (!startPtr) {
        printf("No memory allocated.\n");
        return 0;
    }

    //user variables
    int choice = 0; //users menu choice
    int type = 0; //event type
    int time = 0; //event time
    
    //while loop that controls menu 
    while (choice != 3) {
        
        //get choice from user
        choice = menu();
        
        switch(choice) {
            //User chose to enqueue
            //program will get information for the event and add to the queue
            case 1: {
                printf("Type of event? ");
                scanf("%d", &type);
                printf("Time of event? ");
                scanf("%d", &time);
                EVENT* newEvent = createEvent(type, time);
                enq(&startPtr, newEvent, FIFO);
                printq(&startPtr);
                break;
            }
            //user chose to dequeue
            //first item in queue (not startPtr) will be removed and event will be returned
            case 2: {
                EVENT* deqEvent = deq(&startPtr);
                printq(&startPtr);
                printf("%d ", deqEvent -> time);
                printf("%d\n", deqEvent -> type);
                break;
            }
            //user ends program
            case 3: {
                printf("QUITTING\n");
                return 0;
            }

            case 4: {
                printf("length: %d\n", queueLength(&startPtr));
                break;
            }
        }
    }

    return 0;
}