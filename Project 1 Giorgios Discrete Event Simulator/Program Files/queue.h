//Joe Uzdzinski
//queue header file

#ifndef QUEUE_H
#define QUEUE_H


#include "structs.h"



//type of queue, for enqueue function
typedef enum q_type {FIFO, PRIORITY} Q_TYPE;

void enq (NODE* *startPtr, EVENT* eventPtr, Q_TYPE Q);
EVENT* deq (NODE* *startPtr);
void printq (NODE* *startPtr);
int queueLength (NODE* *startPtr);

#endif