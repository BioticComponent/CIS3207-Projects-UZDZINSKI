
#include "structs.h"


#define DEF_DICT "words.txt"  //default dictionary
#define DEF_PORT 2000
#define Q_MAX 5


FILE* DICT;
int PORT;

int main(int argc, char **argv) {
    printf("HELLO\n");
    //setup port and dictionary
    if (argc == 1) { //no args, make defaults
        PORT = DEF_PORT;
        if (!(DICT = fopen(DEF_DICT, "r"))) {
            printf("dictionary error\n");
            return 0;
        }
    } else if (argc == 2) { //1 arg, did they pass a port or dict?
        PORT = atoi(argv[1]); //assume port
        if (PORT < 1024 || PORT > 65535) { //port out of range? assume dict
            if (!(DICT = fopen(argv[1], "r"))) {
                printf("port not in range (1024,65535) or dictionary error\n");
                return 0;
            }
            PORT = DEF_PORT; //dictionary worked, set port to default
        }
    } else if (argc == 3) { //2 args, passed port and dictionary
        PORT = atoi(argv[1]);
        if (PORT < 1024 || PORT > 65535) { //if true, port out of range
            printf("port not in range (1024,65535)\n");
            return 0;
        }
        if (!(DICT = fopen(argv[2], "r"))) {
            printf("dictionary error\n");
            return 0;
        }
    } else { //more than 2 arguments
        printf("too many arguments\n");
        return 0;
    }

    // setup connection socket with selected port
    int connectionSocket;
    if ((connectionSocket = open_listenfd(PORT)) == -1) {
        printf("error connecting to port, try again\n");
        return 0;
    }

    //setup server struct and its included queues
    SERVER* SVR = malloc(sizeof(SERVER));
    SVR -> job_Qptr = malloc(sizeof(JN)); //JN is job node
    SVR -> job_Qptr -> nextPtr = NULL;
    SVR -> log_Qptr = malloc(sizeof(LN)); //LN is log node
    SVR -> log_Qptr -> nextPtr = NULL;
    //check for memory allocation failure
    if ( !SVR || !(SVR -> job_Qptr) || !(SVR -> log_Qptr) ) {
        printf("memory allocaton failure\n");
        return 0;
    }
    if ( (pthread_mutex_init(&SVR -> job_mutex, NULL)) || (pthread_mutex_init(&SVR -> log_mutex, NULL)) || (pthread_cond_init(&SVR->jobCV_notEmpty, NULL)) || (pthread_cond_init(&SVR->jobCV_notFull, NULL)) || (pthread_cond_init(&SVR->logCV_notEmpty, NULL)) || (pthread_cond_init(&SVR->logCV_notFull, NULL)) ) {
        printf("lock or cv initialization failure\n");
        return 0;
    }

    //setup worker threads
    pthread_t workers[Q_MAX];
    for (int i = 0; i < Q_MAX; i++) {
        pthread_create(&workers[i], NULL, workerThreadRoutine, (void*) SVR);
    }

    int clientSocket;
    // struct sockaddr_in client;
	// int clientLen = sizeof(client);
    char* clientMessage = "Hello! I hope you can see this.\n";
	char* msgRequest = "Send me some text and I'll respond with something interesting!\nSend the escape key to close the connection.\n";

    while (1) {
        if((clientSocket = accept(connectionSocket, NULL, NULL)) == -1){
            printf("Error connecting to client.\n");
            return 0;
	    }
        printf("client connected\n");

        send(clientSocket, clientMessage, strlen(clientMessage), 0);
	    send(clientSocket, msgRequest, strlen(msgRequest), 0);

        pthread_mutex_lock(&SVR -> job_mutex);

        while (qlength_J(&SVR -> job_Qptr) == Q_MAX) {
            pthread_cond_wait(&SVR -> jobCV_notFull, &SVR -> job_mutex);
        }

        putClient(SVR, clientSocket);

        pthread_cond_signal(&SVR -> jobCV_notEmpty);

        pthread_mutex_unlock(&SVR -> job_mutex);
    }

    // printf("%d\n", deq_J(&SVR->job_Qptr));
    // enq_J(&SVR->job_Qptr, 1);
    // enq_J(&SVR->job_Qptr, 2);
    // enq_J(&SVR->job_Qptr, 3);
    // printf("%d\n", qlength_J(&SVR->job_Qptr));
    // printf("%d\n", deq_J(&SVR->job_Qptr));
    // printq_J(&SVR->job_Qptr);

    // printf("%s\n", deq_L(&SVR->log_Qptr));
    // enq_L(&SVR->log_Qptr, "hello");
    // enq_L(&SVR->log_Qptr, "goodbye");
    // enq_L(&SVR->log_Qptr, "goodnight");
    // printf("%d\n", qlength_L(&SVR->log_Qptr));
    // char* log = deq_L(&SVR->log_Qptr);
    // printf("%s\n", log);
    // printq_L(&SVR->log_Qptr);

    return 0;
}