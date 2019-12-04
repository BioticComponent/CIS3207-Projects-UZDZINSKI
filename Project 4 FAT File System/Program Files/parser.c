//Joe Uzdzinski
//parcer.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "structs.h"
#include "internal.h"
#include "external.h"
#include "parser.h"


//get and parses command into char** argsPtr from user
int getParsedCommand(char** argsPtr, ENV* env, CMDINFO* cmdinfo) {

    //copying currDir so it is not effected by strtok
    char * tempDir = malloc(sizeof(char)*300);
    strcpy(tempDir, env -> currDir);
    
    
    //tokenize current working directory, print last token as current directory for prompt
    char * dirTok = strtok(tempDir, "/");
    char * tempTok = dirTok;
    while (dirTok != NULL) {
        tempTok = dirTok;
        dirTok = strtok(NULL, "/");
    }
    
    //print shell prompt, prompting user to enter command
    printf("%s MyShell$ ", tempTok);
    
    //get command
    char* command = malloc(sizeof(char)*100);
    scanf(" %[^\n]s", command);
    
    //parse command and put it into argsPtr
    char* token = strtok(command, " \t;");
    int i = 0;
    int tokLen = 0;
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            cmdinfo -> inputRedirect++;
        }
        if (strcmp(token, ">") == 0) {
            cmdinfo -> outputTruncate++;
        }
        if (strcmp(token, ">>") == 0) {
            cmdinfo -> outputAppend++;
        }
        if (strcmp(token, "|") == 0) {
            cmdinfo -> pipe++;
        }
        if (strcmp(token, "&") == 0) {
            cmdinfo -> amp++;
        }
        tokLen = strlen(token);
        argsPtr[i] = (char*)malloc(tokLen + 1);
        strcpy(argsPtr[i],token);
        token = strtok(NULL, " \t;");
        i++;
    }

    //set dualRedirect if both < and > are present in command
    //reset input and output variables
    if ((cmdinfo -> inputRedirect) + (cmdinfo -> outputTruncate) == 2) {
        cmdinfo -> dualRedirect = 1;
        cmdinfo -> inputRedirect = 0;
        cmdinfo -> outputTruncate = 0;
    }

    //free allocated memory
    free(command);
    free(tempDir);

    //mark end of args
    argsPtr[i+1] = NULL;

    //check if more than one incompatible special operations in command
    int specialCharCheck = 0;
    int specialCharCheck2 = 0;
    int specialCharCheck3 = 0;
    specialCharCheck = (cmdinfo -> inputRedirect) + (cmdinfo -> outputAppend) + (cmdinfo -> pipe) + (cmdinfo -> amp);
    specialCharCheck2 = (cmdinfo -> outputTruncate) + (cmdinfo -> outputAppend) + (cmdinfo -> pipe) + (cmdinfo -> amp);
    specialCharCheck2 = (cmdinfo -> dualRedirect) + (cmdinfo -> outputAppend) + (cmdinfo -> pipe) + (cmdinfo -> amp);

    char c = argsPtr[0][0];
    if ( (!isalpha(c)) && (argsPtr[0][1] != '/') ) {
        puts("Invalid command.");
        //return unsuccessful command
        return -1;
    }

   if (specialCharCheck > 1 || specialCharCheck2 > 1 || specialCharCheck3 > 1) {
        puts("Invalid command.");
        //return unsuccessful command
        return -1;
    }

    //return i, which is equivalent to number of arguments
    return i;
}

int getBatchCommand(char * batchCommand, char ** argsPtr, ENV* env, CMDINFO* cmdinfo) {
    
    //copying currDir so it is not effected by strtok
    char * tempDir = malloc(sizeof(char)*300);
    strcpy(tempDir, env -> currDir);

    //tokenize current working directory, print last token as current directory for prompt
    char * dirTok = strtok(tempDir, "/");
    char * tempTok = dirTok;
    while (dirTok != NULL) {
        tempTok = dirTok;
        dirTok = strtok(NULL, "/");
    }
    
    //print shell prompt, prompting user to enter command
    printf("%s MyShell$ %s\n", tempTok, batchCommand);

    //parse command and put it into argsPtr
    char* token = strtok(batchCommand, " \t;");
    int i = 0;
    int tokLen = 0;
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            cmdinfo -> inputRedirect++;
        }
        if (strcmp(token, ">") == 0) {
            cmdinfo -> outputTruncate++;
        }
        if (strcmp(token, ">>") == 0) {
            cmdinfo -> outputAppend++;
        }
        if (strcmp(token, "|") == 0) {
            cmdinfo -> pipe++;
        }
        if (strcmp(token, "&") == 0) {
            cmdinfo -> amp++;
        }
        tokLen = strlen(token);
        argsPtr[i] = (char*)malloc(tokLen + 1);
        strcpy(argsPtr[i],token);
        token = strtok(NULL, " \t;");
        i++;
    }
    //set dualRedirect if both < and > are present in command
    //reset input and output variables
    if ((cmdinfo -> inputRedirect) + (cmdinfo -> outputTruncate) == 2) {
        cmdinfo -> dualRedirect = 1;
        cmdinfo -> inputRedirect = 0;
        cmdinfo -> outputTruncate = 0;
    }

    //free allocated memory
    free(tempDir);

    //mark end of args
    argsPtr[i+1] = NULL;

    //check if more than one incompatible special operations in command
    int specialCharCheck = 0;
    int specialCharCheck2 = 0;
    int specialCharCheck3 = 0;
    specialCharCheck = (cmdinfo -> inputRedirect) + (cmdinfo -> outputAppend) + (cmdinfo -> pipe) + (cmdinfo -> amp);
    specialCharCheck2 = (cmdinfo -> outputTruncate) + (cmdinfo -> outputAppend) + (cmdinfo -> pipe) + (cmdinfo -> amp);
    specialCharCheck2 = (cmdinfo -> dualRedirect) + (cmdinfo -> outputAppend) + (cmdinfo -> pipe) + (cmdinfo -> amp);

    char c = argsPtr[0][0];
    if ( (!isalpha(c)) && (argsPtr[0][1] != '/') ) {
        puts("Invalid command.");
        //return unsuccessful command
        return -1;
    }

    if (specialCharCheck > 1 || specialCharCheck2 > 1 || specialCharCheck3 > 1) {
        puts("Invalid command.");
        //return unsuccessful command
        return -1;
    }

    //return i, which is equivalent to number of arguments
    return i;

}


//prints each element
void printArray(char** arrPtr) {
    int j = 0;
    while (arrPtr[j] != NULL) {
        printf("%s ", arrPtr[j]);
        j++;
    }
    printf("\n");
}

//sets each element = to NULL for next iteration
void nullifyArray(char** arrPtr) {
    int j = 0;
    while (arrPtr[j] != NULL) {
        arrPtr[j] = NULL;
        j++;
    }
}

//comparmentalizes each command for ease of use
void splitInputHandler (char** argsPtr, INPUT* splitInput, CMDINFO* cmdinfo) {
    
    //each if statement checks to see if a special operation was present, if it was then a unique 
    //  method of splitting the input will take place
    // the command with arguments will go in splitInput -> command
    // if a pipe is present, the second command with arguments will go in splitInput -> command2
    // input files will be placed in splitInput -> inputFile
    // output files will be places in splitInput -> outputFile

    if (cmdinfo -> inputRedirect == 1) {
        int i = 0;
        int j = 0;
        while (strcmp(argsPtr[i], "<") != 0) {
            splitInput -> command[j] = argsPtr[i];
            i++;
            j++;
        }
        splitInput -> inputFile[0] = argsPtr[i+1];
    
    } else if (cmdinfo -> outputTruncate == 1) {
        int i = 0;
        int j = 0;
        while (strcmp(argsPtr[i], ">") != 0) {
            splitInput -> command[j] = argsPtr[i];
            i++;
            j++;
        }
        splitInput -> outputFile[0] = argsPtr[i+1];
    
    } else if (cmdinfo -> outputAppend == 1) {
        int i = 0;
        while (strcmp(argsPtr[i], ">>") != 0) {
            splitInput -> command[i] = argsPtr[i];
            i++;
        }
        splitInput -> outputFile[0] = argsPtr[i+1];
    
    } else if (cmdinfo -> dualRedirect == 1) {
        int i = 0;
        while ( (strcmp(argsPtr[i], "<") != 0) && (strcmp(argsPtr[i], ">") != 0) ) {
            splitInput -> command[i] = argsPtr[i];
            i++;
        }
        if (strcmp(argsPtr[i], "<") == 0) {
            splitInput -> inputFile[0] = argsPtr[i+1];
            splitInput -> outputFile[0] = argsPtr[i+3];
        } else {
            splitInput -> outputFile[0] = argsPtr[i+1];
            splitInput -> inputFile[0] = argsPtr[i+3];
        }
    
    } else if (cmdinfo -> pipe == 1) {
        int i = 0;
        while (strcmp(argsPtr[i], "|") != 0) {
            splitInput -> command[i] = argsPtr[i];
            i++;
        }
        i++;
        int j = 0;
        while (argsPtr[i] != NULL) {
            splitInput -> command2[j] = argsPtr[i];
            i++;
            j++;
        }
        
    } else if (cmdinfo -> amp == 1) {
        int i = 0;
        while (strcmp(argsPtr[i], "&") != 0) {
            splitInput -> command[i] = argsPtr[i];
            i++;
        }
    
    } else {
        int i = 0;
        while (argsPtr[i] != NULL) {
            splitInput -> command[i] = argsPtr[i];
            i++;
        }
    }
}

//sets up file descriptors according to the command special operations
void fdHandler (INPUT* splitInput, CMDINFO* cmdinfo, FILED* fileD) {
    
    if (cmdinfo -> inputRedirect == 1) {
        fileD -> fd = open(splitInput -> inputFile[0], O_RDONLY);
        dup2(fileD -> fd, 0);
    } else if (cmdinfo -> outputTruncate == 1) {
        fileD -> fd = open(splitInput -> outputFile[0], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        dup2(fileD -> fd, 1);
    } else if (cmdinfo -> outputAppend == 1) {
        fileD -> fd = open(splitInput -> outputFile[0], O_CREAT|O_WRONLY|O_APPEND, S_IRWXU);
        dup2(fileD -> fd, 1);
    } else if (cmdinfo -> dualRedirect == 1) {
        fileD -> fd = open(splitInput -> inputFile[0], O_RDONLY);
        fileD -> fd2 = open(splitInput -> outputFile[0], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        dup2(fileD -> fd, 0);
        dup2(fileD -> fd2, 1);
    }
}

//handles pipes
//fork and execs
//first command becomes child
//second command is parent
void pipeHandler(INPUT* splitInput, ENV* env, CMDINFO* cmdinfo, FILED* fileD) {
    int thePipe[2];
    if (pipe(thePipe) == 0) {
        if (fork() == 0) { //child - first command
            close(1);
            dup2(thePipe[1], 1);
            close(thePipe[0]);
            if (checkInternal(splitInput)) {
                internalCommandHandler(splitInput, env, cmdinfo);
            } else {
                externalCommandHandler(splitInput, cmdinfo, fileD);
            }
            exit(1);
        } else { //parent - second command
            close(0); 
            dup2(thePipe[0], 0);
            close(thePipe[1]);
            //switch command2 into command1 of splitInput because each function in program is designed to work on command1
            nullifyArray(splitInput -> command);
            int i = 0;
            while (splitInput -> command2[i] != NULL ) {
                splitInput -> command[i] = splitInput -> command2[i];
                i++;
            }
            if (checkInternal(splitInput)) {
                internalCommandHandler(splitInput, env, cmdinfo);
            } else {
                externalCommandHandler(splitInput, cmdinfo, fileD);
            }
            close(thePipe[0]); 
            dup2(fileD -> stdinput, 0);
            int status = 0;
            wait(&status);
        }
    }
}