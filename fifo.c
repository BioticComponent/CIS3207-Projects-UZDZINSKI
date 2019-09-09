//Joe Uzdzinski
//test implementation of a fifo queue
//this is just a baseline to use for the Discrete Event Simulator, not actual implementation

#include <stdio.h>
#include <stdlib.h>


//an event struct that holds type and time
typedef struct _EVENT {
    int type;
    int time; 
} EVENT;

//a NODE struct that holds a pointer to an EVENT stuct and a pointer that points to the next NODE
typedef struct _NODE {
    EVENT* eventPtr;
    struct _NODE* nextPtr;
} NODE;



//FUNCTION DECLARATIONS
void enq (NODE* *startPtr, EVENT* eventPtr);
EVENT* deq (NODE* *startPtr);
void printq (NODE* *startPtr);
int menu (void);
EVENT* createEvent (int _type, int _time);



//START OF MAIN
int main() {
    
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
                enq(&startPtr, newEvent);
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
        }
    }

    return 0;
} //END OF MAIN

//FUNCTION DEFINITIONS

//ENQUEUE FUNCTION
//takes a pointer to the startPtr and an event pointer
//since FIFO, element will be put at end of list
void enq (NODE* *startPtr, EVENT* eventPtr) {
    
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
EVENT* deq (NODE* *startPtr) {
    
    //creating a currentPtr to work on instead of startPtr
    NODE* currentPtr = *startPtr;
    
    //creating a temporary pointer to hold the event in the first element
    EVENT* tempPtr = currentPtr -> nextPtr -> eventPtr;
    
    //pointing startPtr to the next element in the queue
    *startPtr = currentPtr -> nextPtr;
    
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
            printf("type: %d time: %d --> ", currentPtr -> eventPtr -> type, currentPtr -> eventPtr -> time);
        }
        counter++;
        currentPtr = currentPtr -> nextPtr;
    }
    //every last pointer points to NULL, so each print ends with NULL
    printf("NULL\n");
}

//MENU FUNCTION
//asks user for choice, returns choice
int menu(void) {
    printf("1. enq\n");
    printf("2. dq\n");
    printf("3. quit\n");
    int choice;
    printf("Enter choice: ");
    scanf("%d", &choice);
    return choice;
}

//CREATES A NEW EVENT
//takes an int type and time for a new event struct
//returns a pointer to the event
EVENT* createEvent (int _type, int _time) {
    EVENT* tempPtr = (EVENT*)malloc(sizeof(EVENT));
    tempPtr -> type = _type;
    tempPtr -> time = _time;
    return tempPtr;
}
