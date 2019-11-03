#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _NODE {
    int sd;
    struct _NODE* nextPtr;
} NODE;

void enq (NODE* *startPtr, int _sd);
int deq (NODE* *startPtr);
void printq (NODE* *startPtr);
int qlength (NODE* *startPtr);

#endif