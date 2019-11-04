#include "structs.h"
#include "queue.h"



void* workerThreadRoutine(void* args) {
    SERVER* SVR = args;
    int bytesReturned;
    char recvBuffer[BUF_LEN];
    recvBuffer[0] = '\0';

    // char* clientMessage = "Hello! I hope you can see this.\n";
	// char* msgRequest = "Send me some text and I'll respond with something interesting!\nSend the escape key to close the connection.\n";
	char* msgResponse = "I actually don't have anything interesting to say...but I know you sent ";
	char* msgPrompt = ">>>";
	char* msgError = "I didn't get your message. ):\n";
	char* msgClose = "Goodbye!\n";

    while (1) {
        pthread_mutex_lock(&SVR -> job_mutex);

        while (qlength_J(&SVR -> job_Qptr) == 0) {
            pthread_cond_wait(&SVR -> jobCV_notEmpty, &SVR -> job_mutex);
        }

        int clientSocket = getClient(SVR);

        pthread_cond_signal(&SVR -> jobCV_notFull);

        pthread_mutex_unlock(&SVR -> job_mutex);

        while (1) {
            send(clientSocket, msgPrompt, strlen(msgPrompt), 0);
            
            bytesReturned = (int) recv(clientSocket, recvBuffer, BUF_LEN, 0);

            //Check if we got a message, send a message back or quit if the
		    //user specified it.
            if(bytesReturned == -1){
                send(clientSocket, msgError, strlen(msgError), 0);
            }
            //'27' is the escape key.
            else if(recvBuffer[0] == 27){
                send(clientSocket, msgClose, strlen(msgClose), 0);
                close(clientSocket);
                break;
            } else {
                send(clientSocket, msgResponse, strlen(msgResponse), 0);
                send(clientSocket, recvBuffer, bytesReturned, 0);
		    }
        }
        close(clientSocket);
    }
    return NULL;
}

int getClient(SERVER* SVR) {
    return deq_J(&SVR -> job_Qptr);
}

void putClient(SERVER* SVR, int clientSocket) {
    enq_J(&SVR -> job_Qptr, clientSocket);
    return;
}

