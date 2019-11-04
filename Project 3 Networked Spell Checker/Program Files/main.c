//joe uzdzinski

#include "utility.h"

#define DEF_DICT "words.txt"    //default dictionary
#define DEF_PORT 3000          //default port
#define NUM_WORKERS 3         //number of workers


int main(int argc, char **argv) {
    //setup port and dictionary
    int PORT;
    if (argc == 1) { //no args, make defaults
        PORT = DEF_PORT;
        if (!(DICT = fopen(DEF_DICT, "r"))) {
            printf("dictionary error\n");
            return 0;
        }
    } else if (argc == 2) { //1 additional arg, did they pass a port or dict?
        PORT = atoi(argv[1]); //assume port
        if (PORT < 1024 || PORT > 65535) { //port out of range? assume dict
            if (!(DICT = fopen(argv[1], "r"))) {
                printf("port not in range (1024,65535) or dictionary error\n");
                return 0;
            }
            PORT = DEF_PORT; //dictionary worked, set port to default
        }
    } else if (argc == 3) { //2 additional args, passed port and dictionary
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

    //setup log file
    if (!(LOG = fopen("log.txt", "w"))) {
        printf("log file error\n");
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
    //check for lock or cv init failure
    if ( (pthread_mutex_init(&SVR -> job_mutex, NULL)) || (pthread_mutex_init(&SVR -> log_mutex, NULL)) || (pthread_cond_init(&SVR->jobCV_notEmpty, NULL)) || (pthread_cond_init(&SVR->jobCV_notFull, NULL)) || (pthread_cond_init(&SVR->logCV_notEmpty, NULL)) || (pthread_cond_init(&SVR->logCV_notFull, NULL)) ) {
        printf("lock or cv initialization failure\n");
        return 0;
    }

    //setup worker threads
    pthread_t workers[NUM_WORKERS];
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_create(&workers[i], NULL, workerThreadRoutine, (void*) SVR);
    }

    //setup logger thread
    pthread_t logger;
    pthread_create(&logger, NULL, loggerThreadRoutine, (void*) SVR);

    //set up client socket
    int clientSocket;
    struct sockaddr_in client;
	int clientLen = sizeof(client);
    //client welcome message
    char* clientMessage = "Welcome to the spell checker.\nInput a word at the prompt (\">>>\") to see if you spelled it correctly.\nEnter the esc key at the prompt to exit.\nIf the prompt does not show, you must wait for an open connection.\n";
    
    while (1) {
        //accept connection
        if((clientSocket = accept(connectionSocket, (struct sockaddr*)&client, &clientLen)) == -1){
            printf("Error connecting to client.\n");
            return 0;
	    }
        printf("connection successful with socket descriptor %d\n", clientSocket);

        //send client welcome message
        send(clientSocket, clientMessage, strlen(clientMessage), 0);

        //lock access to the job queue
        pthread_mutex_lock(&SVR -> job_mutex);

        //wait while the job queue is full
        while (qlength_J(&SVR -> job_Qptr) >= Q_MAX) {
            pthread_cond_wait(&SVR -> jobCV_notFull, &SVR -> job_mutex);
        }

        //put the accepted client socket into the job queue so a worker can attend to it
        putClient(SVR, clientSocket);

        // DEBUG
        // printq_J(&SVR -> job_Qptr);

        //signal to workers that the queue is not empty
        pthread_cond_signal(&SVR -> jobCV_notEmpty);

        //unlock access to the job queue
        pthread_mutex_unlock(&SVR -> job_mutex);
    }

    // DEBUG
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

    close(connectionSocket);
    fclose(DICT);
    fclose(LOG);
    return 0;
}