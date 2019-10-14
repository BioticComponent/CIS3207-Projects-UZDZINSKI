//Joe Uzdzinski
//structs.h

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct _ENVIRONMENT {
    char * currDir;
    char * shellDir;
} ENV;

typedef struct _COMMAND_INFO {
    int amp;
    int inputRedirect;
    int outputTruncate;
    int outputAppend;
    int dualRedirect;
    int pipe;
} CMDINFO;

typedef struct _SPLIT_INPUTS {
    char * command[100];
    char * command2[100];
    char * inputFile[100];
    char * outputFile[100];
} INPUT;

typedef struct _FILE_DESCRIPTORS {
    int fd;
    int fd2;
    int stdinput;
    int stdoutput;
} FILED;


#endif