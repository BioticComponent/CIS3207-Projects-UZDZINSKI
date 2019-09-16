//Joe Uzdzinski
//config source file

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//set variables
void configConstants() {
    
    //declare an array of int to hold the variables from "config.txt"
    int config[12];

    FILE* configPtr = fopen("config.txt", "r");
    if (configPtr == NULL) {
        perror("error opening config file");
        return;
    }

    //opening new log.txt file to copy configuration to log.txt
    //this will overwrite an existing log.txt file
    FILE* logPtr = fopen("log.txt", "w");
    if (logPtr == NULL) {
        perror("error opening log file");
        return;
    }

    //log.txt first line header
    fprintf(logPtr, "Simulation Configuration: \n\n");
    
    char* line = (char*) malloc(64*sizeof(char));
    char* token = (char*) malloc(64*sizeof(char));
    int num = 0;
    int i = 0;
    
    while (fgets(line, 64, configPtr) != NULL) {
        fprintf(logPtr, "%s", line); //print the config file to "log.txt"
        token = strchr(line, ' '); //parsing for setting variables
        num = atoi(token);
        config[i] = num;
        i++;
    }
    fprintf(logPtr, "\n\n");
    fprintf(logPtr, "Simulation Log: \n\n");

    //closing files
    fclose(configPtr);
    fclose(logPtr);

    //define the variables
    SEED = config[0];
    INIT_TIME = config[1];
    FIN_TIME = config[2];
    ARRIVE_MIN = config[3];
    ARRIVE_MAX = config[4];
    QUIT_PROB = config[5];
    CPU_MIN = config[6];
    CPU_MAX = config[7];
    DISK1_MIN = config[8];
    DISK1_MAX = config[9];
    DISK2_MIN = config[10];
    DISK2_MAX = config[11];
}

//prints the passed in event to "log.txt"
void printToLog(EVENT* thisEvent) {
    
    FILE* logPtr = fopen("log.txt", "a");
    if (logPtr == NULL) {
        perror("error opening log file");
        return;
    }

    fprintf(logPtr, "At time %d, process %d ", thisEvent -> time, thisEvent -> jobNo);

    switch (thisEvent -> type) {
            case PROCESS_ARRIVAL: {
                fprintf(logPtr, "arrives at the CPU queue\n");
                break;
            }
            case CPU_BEGIN: {
                fprintf(logPtr, "begins at the CPU\n");
                break;
            }
            case CPU_FINISH: { 
                fprintf(logPtr, "finishes at the CPU\n"); 
                break;
            }
            case PROCESS_EXIT: { 
                fprintf(logPtr, "exits the system\n");
                break;
            }
            case DISK1_ARRIVAL: { 
                 fprintf(logPtr, "arrives at the disk1 queue\n");               
                break;
            }
            case DISK1_BEGIN: { 
                 fprintf(logPtr, "begins at disk1\n");               
                break;
            }
            case DISK2_ARRIVAL: {
                fprintf(logPtr, "arrives at the disk2 queue\n");                
                break;
            }
            case DISK2_BEGIN: {
                fprintf(logPtr, "begins at disk2\n");                
                break;
            }
            case DISK1_FINISH: { 
                fprintf(logPtr, "finishes at disk1\n");
                break;
            }
            case DISK2_FINISH: { 
                fprintf(logPtr, "finishes at disk2\n");
                break;
            }
            case SIMULATION_FINISH: { 
                fprintf(logPtr, "terminates the simulation\n");
                break;
            }
    }

    fclose(logPtr);

} 

