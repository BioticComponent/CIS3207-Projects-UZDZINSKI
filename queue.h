#ifndef FIFO_H
#define FIFO_H

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

//type of queue, for enqueue function
typedef enum q_type {FIFO, PRIORITY} Q_TYPE;

void enq (NODE* *startPtr, EVENT* eventPtr, Q_TYPE Q);
EVENT* deq (NODE* *startPtr);
void printq (NODE* *startPtr);
int menu (void);
EVENT* createEvent (int _type, int _time);
int queueLength (NODE* *startPtr);

#endif