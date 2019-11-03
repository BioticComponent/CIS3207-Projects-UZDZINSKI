#include "queue.h"



//ENQUEUE FUNCTION
//takes a pointer to the startPtr, an event pointer, and type of queue
void enq (NODE* *startPtr, int _sd) {
    
    //allocating memory for newPtr
    NODE* newPtr = malloc(sizeof(NODE));
    
    //testing for NULL
    if (!newPtr) {
        printf("No memory allocated.\n");
        return;
    }

    //setting members of newPtr
    newPtr -> sd = _sd;
    newPtr -> nextPtr = NULL;    

    //creating a currentPtr to work on instead of startPtr
    NODE* currentPtr = *startPtr;
    //shifting currentPtr to last element
    while ((currentPtr -> nextPtr) != NULL) {
        currentPtr = currentPtr -> nextPtr;
    }

    //pointing last element to newPtr
    currentPtr -> nextPtr = newPtr;
}

//DEQUEUE FUNCTION
//removes the first in element
//takes a pointer to startPtr of queue
//returns a pointer to the event that was in the removed element
int deq (NODE* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    NODE* currentPtr = *startPtr;
    
    //creating a temporary pointer to hold the event in the first element
    // EVENT* tempPtr = NULL;
    int sd;

    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        sd = -1;
    } else { 
        
        sd = currentPtr -> nextPtr -> sd;

        //pointing startPtr to the next element in the queue
        *startPtr = currentPtr -> nextPtr;
        
    }
    //return socket descriptor
    return sd;
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
            printf("sd: %d --> ", currentPtr -> sd);
        }
        counter++;
        currentPtr = currentPtr -> nextPtr;
    }
    //every last pointer points to NULL, so each print ends with NULL
    printf("NULL\n");
}

//LENGTH OF QUEUE FUNCTION
int qlength (NODE* *startPtr) {
    
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