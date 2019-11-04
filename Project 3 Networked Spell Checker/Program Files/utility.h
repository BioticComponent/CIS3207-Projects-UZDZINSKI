#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_LEN 256         //buffer length
#define Q_MAX 5             //max q length

FILE* DICT;                 //dictionary global variable
FILE* LOG;                  //log global variable

typedef struct JOB_NODE {           //job node struct for job queue
    int sd;
    struct JOB_NODE* nextPtr;
} JN;

typedef struct LOG_NODE {           //log node struct for log queue
    char* log;
    struct LOG_NODE* nextPtr;
} LN;

typedef struct _SERVER {            //server struct to hold queues, mutual exclusion, and condition variables
    JN* job_Qptr;
    LN* log_Qptr;
    pthread_mutex_t job_mutex, log_mutex;
    pthread_cond_t jobCV_notEmpty, jobCV_notFull;
    pthread_cond_t logCV_notEmpty, logCV_notFull;
} SERVER;

//function declarations
int open_listenfd(int);
void* workerThreadRoutine(void* args);
void* loggerThreadRoutine(void* args);
int spellCheck(char* word);
int getClient(SERVER* SVR);
void putClient(SERVER* SVR, int clientSocket);
char* getLog(SERVER* SVR);
void putLog(SERVER* SVR, char* logMsg);
char* itoa(int value, char* result, int base);
void enq_J (JN* *startPtr, int _sd);
int deq_J (JN* *startPtr);
void printq_J (JN* *startPtr);
int qlength_J (JN* *startPtr);
void enq_L (LN* *startPtr, char* _log);
char* deq_L (LN* *startPtr);
void printq_L (LN* *startPtr);
int qlength_L (LN* *startPtr);

#endif