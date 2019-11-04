//joe uzdzinski

#include "utility.h"

//worker threads are directed to this routine
//they will execute in a loop until client enters esc key into prompt
void* workerThreadRoutine(void* args) {
    SERVER* SVR = args;
    int bytesReturned; //check for successful message
    char* word; // word from client
    char verdict[32]; // OK/MISPELLED
    char logMsg[128]; // word + " : " + verdict

	char* msgPrompt = ">>>";
	char* msgError = "message not received\n";
	char* msgClose = "closing spell checker\n";

    while (1) {
        
        //lock access to the job queue
        pthread_mutex_lock(&SVR -> job_mutex);

        //wait while the job queue is empty
        while (qlength_J(&SVR -> job_Qptr) == 0) {
            pthread_cond_wait(&SVR -> jobCV_notEmpty, &SVR -> job_mutex);
        }

        //removes next job and returns client socket descriptor
        int clientSocket = getClient(SVR);

        // DEBUG
        // printq_J(&SVR -> job_Qptr);

        //signal that the job queue can no longer be full
        pthread_cond_signal(&SVR -> jobCV_notFull);

        //unlock access to the job queue
        pthread_mutex_unlock(&SVR -> job_mutex);

        while (1) {
            //reset 
            word = calloc(BUF_LEN, 1);
            for (int i = 0; i < strlen(logMsg); i++) {
                logMsg[i] = '\0';
            }

            //send prompt ">>>" message
            send(clientSocket, msgPrompt, strlen(msgPrompt), 0);

            //receive word from client
            bytesReturned = (int) recv(clientSocket, word, BUF_LEN, 0);

            //what to do with message?
            if(bytesReturned == -1){ //message error
                send(clientSocket, msgError, strlen(msgError), 0);
            } else if (word[0] == 27) { //'27' is the escape key. QUIT
                send(clientSocket, msgClose, strlen(msgClose), 0);
                close(clientSocket);
                break;
            } else { //successful message, is their word spelled correctly?
                if (spellCheck(word)) { //returns 1 if found, 0 if not
                    strcpy(verdict,"OK\n");
                } else {
                    strcpy(verdict,"MISSPELLED\n");
                }
                //send verdict to client
                send(clientSocket, verdict, strlen(verdict), 0);

                //put verdict into logMsg for log
                strncat(logMsg, word, strlen(word) - 1);
                strcat(logMsg, " : ");
                strcat(logMsg, verdict);
                
                // DEBUG
                // printf("%s", logMsg);

                //deep copy log message
                char* logMsgSave = malloc(sizeof(char) * 128);
                strcpy(logMsgSave, logMsg);

                //lock access to log queue
                pthread_mutex_lock(&SVR -> log_mutex);

                //wait while log queue is full
                while (qlength_L(&SVR -> log_Qptr) >= Q_MAX) {
                    pthread_cond_wait(&SVR -> logCV_notFull, &SVR -> log_mutex);
                }

                //put logMsg into log queue
                putLog(SVR, logMsgSave);

                // DEBUG
                // printq_L(&SVR -> log_Qptr);

                //signal to logger thread that the log queue is no longer empty
                pthread_cond_signal(&SVR -> logCV_notEmpty);

                //unlock access to the log queue
                pthread_mutex_unlock(&SVR -> log_mutex);
            }
        }
        close(clientSocket);
    }
    return NULL;
}

void* loggerThreadRoutine(void* args) {
    SERVER* SVR = args;

    while (1) {
        //lock access to the log queue
        pthread_mutex_lock(&SVR -> log_mutex);

        //wait while the log queue is empty
        while (qlength_L(&SVR -> log_Qptr) == 0) {
            pthread_cond_wait(&SVR -> logCV_notEmpty, &SVR -> log_mutex);
        }

        //get log message, removes log message from queue
        char* logMsg = getLog(SVR);

        // DEBUG
        // printq_L(&SVR -> log_Qptr);
        
        //print log message to log.txt
        fprintf(LOG, "%s", logMsg);
        fflush(LOG);

        //signal that log queue is no longer full
        pthread_cond_signal(&SVR -> logCV_notFull);

        //unlock access to the log queue
        pthread_mutex_unlock(&SVR -> log_mutex);
    }
}

//removes first client from the queue
//returns the client socket descriptor
int getClient(SERVER* SVR) {
    return deq_J(&SVR -> job_Qptr);
}

//adds a client to the queue
void putClient(SERVER* SVR, int clientSocket) {
    enq_J(&SVR -> job_Qptr, clientSocket);
    return;
}

//removes first log from the queue
//returns the log message
char* getLog(SERVER* SVR) {
    return deq_L(&SVR -> log_Qptr);
}

//adds a logMsg to the queue
void putLog(SERVER* SVR, char* logMsg) {
    enq_L(&SVR -> log_Qptr, logMsg);
    return;
}

//returns 1 if word was found, 0 if not
int spellCheck(char* word) {
    int found = 0;
    char dictWord[BUF_LEN];

    //format word for comparison
    int len = strlen(word);
    word[len - 2] = '\n';
    word[len - 1] = '\0';
    
    //compare word to each word in dictionary (dictWord)
    while(fgets(dictWord, BUF_LEN, DICT) != NULL) {
        //if found, break from loop
        if(strcmp(dictWord, word) == 0) {
            found = 1;
            break;
        }
    }
    //set DICT pointer back to beginning of dictionary
    fseek(DICT, 0, SEEK_SET);
    return found;
}

//grabbed off stack overflow, convert int to string
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

