//Joe Uzdzinski
//queue source file

#include "queue.h"
#include "event.h"
#include <stdio.h>
#include <stdlib.h>


//FUNCTION DEFINITIONS

//ENQUEUE FUNCTION
//takes a pointer to the startPtr, an event pointer, and type of queue
void enq (NODE* *startPtr, EVENT* eventPtr, Q_TYPE Q) {
    
    //allocating memory for newPtr
    NODE* newPtr = malloc(sizeof(NODE));
    
    //testing for NULL
    if (!newPtr) {
        printf("No memory allocated.\n");
        return;
    }

    //setting members of newPtr
    newPtr -> eventPtr = eventPtr;
    newPtr -> nextPtr = NULL;    

    //creating a currentPtr to work on instead of startPtr
    NODE* currentPtr = *startPtr;
    if (Q == FIFO) {
        //shifting currentPtr to last element
        while ((currentPtr -> nextPtr) != NULL) {
            currentPtr = currentPtr -> nextPtr;
        }

        //pointing last element to newPtr
        currentPtr -> nextPtr = newPtr;
    } else if (Q == PRIORITY) {
        //temporary pointer to hold position for newPtr
        NODE* tempPtr = malloc(sizeof(NODE));
        
        //putting newPtr's event time into variable to compare in while loop
        int time = newPtr -> eventPtr -> time;

        //cycle through queue until we find a time larger than newPtr
        //  if no time larger found, then placed at end of queue
        while ((currentPtr -> nextPtr) != NULL) {      
            //if statement checks the time of the next element 
            if ((currentPtr -> nextPtr -> eventPtr -> time) > time) {
                //temp ptr holds pointer to nextPtr, so we can place newPtr in the queue      
                tempPtr = currentPtr -> nextPtr;
                currentPtr -> nextPtr = newPtr;
                newPtr -> nextPtr = tempPtr;
                return;
            } 
            //make current pointer point to next element
            currentPtr = currentPtr -> nextPtr; 
        }
        //if function doesnt return by now then we havent found a larger time
        // and element will be placed at the end of the queue
        currentPtr -> nextPtr = newPtr;
    }
    
}

//DEQUEUE FUNCTION
//removes the first in element
//takes a pointer to startPtr of queue
//returns a pointer to the event that was in the removed element
EVENT* deq (NODE* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    NODE* currentPtr = *startPtr;
    
    //creating a temporary pointer to hold the event in the first element
    EVENT* tempPtr = NULL;

    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        tempPtr = NULL;
    } else { 
        
        tempPtr = currentPtr -> nextPtr -> eventPtr;

        //pointing startPtr to the next element in the queue
        *startPtr = currentPtr -> nextPtr;
        
    }
    //return the event
    return tempPtr;
}

//PRINT THE QUEUE
//takes a pointer to the startPtr of the queue
void printq (NODE* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    NODE* currentPtr = *startPtr;
    
    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        printf("Queue is empty\n");
        return;
    }
    
    //counter is created to avoid segfault with trying to access startPtr's event (which doesnt exist)
    int counter = 0;
    while (currentPtr != NULL) {
        if (counter > 0) {
            printf("job %d ", currentPtr -> eventPtr-> jobNo);
            printEventType(currentPtr -> eventPtr -> type);
            printf(" %d --> \n", currentPtr -> eventPtr -> time);
        }
        counter++;
        currentPtr = currentPtr -> nextPtr;
    }
    //every last pointer points to NULL, so each print ends with NULL
    printf("NULL\n");
}

//LENGTH OF QUEUE FUNCTION
int queueLength (NODE* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    NODE* currentPtr = *startPtr;

    int length = 0;

    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        return length;
    }

    while (currentPtr != NULL) {
        length++;
        currentPtr = currentPtr -> nextPtr;
    }

    return length - 1;   
}
