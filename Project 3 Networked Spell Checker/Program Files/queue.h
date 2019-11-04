#ifndef QUEUE_H
#define QUEUE_H

// typedef struct _NODE {
//     int sd;
//     struct _NODE* nextPtr;
// } NODE;

typedef struct JOB_NODE {
    int sd;
    struct JOB_NODE* nextPtr;
} JN; //job node

typedef struct LOG_NODE {
    char * log;
    struct LOG_NODE* nextPtr;
} LN; //log node

void enq_J (JN* *startPtr, int _sd);
int deq_J (JN* *startPtr);
void printq_J (JN* *startPtr);
int qlength_J (JN* *startPtr);

void enq_L (LN* *startPtr, char _log[100]);
char* deq_L (LN* *startPtr);
void printq_L (LN* *startPtr);
int qlength_L (LN* *startPtr);

#endif