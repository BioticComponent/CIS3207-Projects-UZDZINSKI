//joe uzdzinski

#include "utility.h"


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

//JOB DEQUEUE FUNCTION
//removes the first element
//takes a pointer to startPtr of queue
//returns socket decriptor
int deq_J (JN* *startPtr) {
    
    //hold socket desciptor
    int sd; 

    //creating a currentPtr to work on instead of startPtr
    JN* currentPtr = *startPtr;
    
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

//PRINT THE JOB QUEUE
//takes a pointer to the startPtr of the queue
void printq_J (JN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    JN* currentPtr = *startPtr;
    
    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        printf("Queue is empty\n");
        return;
    }
    
    //counter is created to avoid segfault with trying to access startPtr's sd (which doesnt exist)
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

//LENGTH OF JOB QUEUE FUNCTION
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
void enq_L (LN* *startPtr, char* _log) {
    
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
//removes the first element
//takes a pointer to startPtr of queue
//returns the log message stored in the first element
char* deq_L (LN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    LN* currentPtr = *startPtr;
    
    //_log holds the log message that will be returned
    char* _log = malloc(sizeof(char)*100);

    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        _log = "queue empty";
    } else { 
        _log = (currentPtr -> nextPtr -> log);

        //pointing startPtr to the next element in the queue
        *startPtr = currentPtr -> nextPtr;
        
    }
    //return log message
    return _log;
}

//PRINT THE LOG QUEUE
//takes a pointer to the startPtr of the queue
void printq_L (LN* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    LN* currentPtr = *startPtr;
    
    //checking queue for empty
    if (currentPtr == NULL || currentPtr -> nextPtr == NULL) {
        printf("Queue is empty\n");
        return;
    }
    
    //counter is created to avoid segfault with trying to access startPtr's log (which doesnt exist)
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

//LENGTH OF LOG QUEUE FUNCTION
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