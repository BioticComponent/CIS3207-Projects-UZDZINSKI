
#include "structs.h"


//ENQUEUE FUNCTION
//takes a pointer to the startPtr, an event pointer, and type of queue
void enq_J (JN* *startPtr, int _sd) {
    
    //allocating memory for newPtr
    JN* newPtr = malloc(sizeof(JN));
    
    //testing for NULL
    if (!newPtr) {
        printf("No memory allocated.\n");
        return;
    }

    //setting members of newPtr
    newPtr -> sd = _sd;
    newPtr -> nextPtr = NULL;    

    //creating a currentPtr to work on instead of startPtr
    JN* currentPtr = *startPtr;
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
int deq_J (JN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    JN* currentPtr = *startPtr;
    
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
void printq_J (JN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    JN* currentPtr = *startPtr;
    
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
int qlength_J (JN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    JN* currentPtr = *startPtr;

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

///////////////////////////////////////////////////////////////////////////////////////////

//LOG ENQUEUE FUNCTION
//takes a pointer to the startPtr, an event pointer, and type of queue
void enq_L (LN* *startPtr, char _log[100]) {
    
    //allocating memory for newPtr
    LN* newPtr = malloc(sizeof(LN));
    
    //testing for NULL
    if (!newPtr) {
        printf("No memory allocated.\n");
        return;
    }

    //setting members of newPtr
    newPtr -> log = _log;
    newPtr -> nextPtr = NULL;    

    //creating a currentPtr to work on instead of startPtr
    LN* currentPtr = *startPtr;
    //shifting currentPtr to last element
    while ((currentPtr -> nextPtr) != NULL) {
        currentPtr = currentPtr -> nextPtr;
    }

    //pointing last element to newPtr
    currentPtr -> nextPtr = newPtr;
}

//LOG DEQUEUE FUNCTION
//removes the first in element
//takes a pointer to startPtr of queue
//returns a pointer to the event that was in the removed element
char* deq_L (LN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    // LN* currentPtr = malloc(sizeof(startPtr));
    LN* currentPtr = *startPtr;
    
    //creating a temporary pointer to hold the event in the first element
    // EVENT* tempPtr = NULL;
    char* _log = malloc(sizeof(char)*100);

    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        _log = "queue empty";
    } else { 
        _log = (currentPtr -> nextPtr -> log);
        // sd = currentPtr -> nextPtr -> sd;

        //pointing startPtr to the next element in the queue
        *startPtr = currentPtr -> nextPtr;
        
    }
    //return socket descriptor
    return _log;
}

//PRINT THE QUEUE
//takes a pointer to the startPtr of the queue
void printq_L (LN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    LN* currentPtr = *startPtr;
    
    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        printf("Queue is empty\n");
        return;
    }
    
    //counter is created to avoid segfault with trying to access startPtr's event (which doesnt exist)
    int counter = 0;
    while (currentPtr != NULL) {
        if (counter > 0) {
            printf("log: %s --> ", currentPtr -> log);
        }
        counter++;
        currentPtr = currentPtr -> nextPtr;
    }
    //every last pointer points to NULL, so each print ends with NULL
    printf("NULL\n");
}

//LENGTH OF QUEUE FUNCTION
int qlength_L (LN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    LN* currentPtr = *startPtr;

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