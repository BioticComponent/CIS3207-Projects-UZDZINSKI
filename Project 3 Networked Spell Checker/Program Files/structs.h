#ifndef STRUCTS_H
#define STRUCTS_H

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

////////////////////////////
////////open_listen/////////
////////////////////////////
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#define BUF_LEN 512
int open_listenfd(int);
////////////////////////////

typedef struct _SERVER {
    JN* job_Qptr;
    LN* log_Qptr;
    pthread_mutex_t job_mutex, log_mutex;
    pthread_cond_t jobCV_notEmpty, jobCV_notFull;
    pthread_cond_t logCV_notEmpty, logCV_notFull;
} SERVER;

////////////////////////////
////workerThreadRoutine/////
////////////////////////////
void* workerThreadRoutine(void* args);
int getClient(SERVER* SVR);
void putClient(SERVER* SVR, int clientSocket);
////////////////////////////

#endif