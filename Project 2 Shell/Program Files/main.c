//Joe Uzdzinski
//main.c

//This program implements a basic shell, for more information, please see the documentation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "structs.h"
#include "internal.h"
#include "external.h"
#include "parser.h"


int main(int argc, char** argv) {

    //getting startind wd and save to currDir and shellDir
    char * buffer = malloc(sizeof(char)*200);
    getcwd(buffer, sizeof(char)*200);

    char * shellDirBuf = malloc(sizeof(char)*200);
    strcpy(shellDirBuf,buffer);

    ENV * env = malloc(sizeof(ENV));
    env -> currDir = buffer;
    env -> shellDir = shellDirBuf;

    //holds parsed command
    char * argsPtr[100];
    //return value for command handlers
    int numArgs = 0;
    //is internal command? variable. 0: no, 1: yes. starts as 0
    int intCommand = 0;

    //batch file pointer initialization
    FILE* batchPtr = NULL;
    //open batch file if second argument given
    if (argc == 2) {
        batchPtr = fopen(argv[1], "r");
        if (batchPtr == NULL) {
            puts("Unable to read batchfile, continue with user commands\n");
            fclose(batchPtr);
            argc = 1;
        }
    }
    
    //will run until internal command 'exit' or 'quit' is called or end of batch file is reached
    while (1) {
        
        //initialize command information
        CMDINFO* cmdinfo = malloc(sizeof(CMDINFO));
        cmdinfo -> amp = 0;
        cmdinfo -> inputRedirect = 0;
        cmdinfo -> outputTruncate = 0;
        cmdinfo -> outputAppend = 0;
        cmdinfo -> dualRedirect = 0;
        cmdinfo -> pipe = 0;

        //initialize file descriptor variables
        FILED* fileD = malloc(sizeof(FILED));
        fileD -> fd = -1;
        fileD -> fd2 = -1;
        fileD -> stdinput = dup(0);
        fileD -> stdoutput = dup(1);

        //splitInput will hold the different segments of each command
        INPUT* splitInput = malloc(sizeof(INPUT));


        char * batchCommand = malloc(sizeof(char)*100);
        if (argc == 2) { //get batch commands
            if (fgets(batchCommand, 100, batchPtr) == NULL) {
                exit(1);
            } else {
                char *end = strstr(batchCommand, "\n");
                if (end != NULL) {
                    strcpy(end, "\0");
                }
            }
            numArgs = getBatchCommand(batchCommand, argsPtr, env, cmdinfo);

        } else { //get user commands
            //get parsed command from user and put into argsPtr
            //returns number of arguments
            //returns -1 if invalid number of special operations
            numArgs = getParsedCommand(argsPtr, env, cmdinfo);
        }

        
        
        if (numArgs == -1) {
            ;//invalid command, skip command handler, get another command
        
        //command handler
        } else {
            
            //compartmentalizes command
            splitInputHandler(argsPtr, splitInput, cmdinfo);

            if (cmdinfo -> pipe == 1) { //pipe present
                pipeHandler(splitInput, env, cmdinfo, fileD);

            } else { //no pipe
                //check if internal
                if ((intCommand = checkInternal(splitInput)) == 1) {
                    fdHandler(splitInput, cmdinfo, fileD);
                    internalCommandHandler(splitInput, env, cmdinfo);
                    dup2(fileD -> stdinput, 0);
                    dup2(fileD -> stdoutput, 1);
                    close(fileD -> fd);
                    close(fileD -> fd2);

                } else { //external command
                    externalCommandHandler(splitInput, cmdinfo, fileD);
                }
            }
        }
    
            
        // iteration cleanup
        nullifyArray(argsPtr);
        nullifyArray(splitInput -> command);
        nullifyArray(splitInput -> command2);
        nullifyArray(splitInput -> inputFile);
        nullifyArray(splitInput -> outputFile);
        free(cmdinfo);
        free(splitInput);
        free(batchCommand);
        
    }

    if (argc == 2) {
        fclose(batchPtr);
    }
    free(buffer);
    free(shellDirBuf);

    return 0;
    
}