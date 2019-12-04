//Joe Uzdzinski
//external.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "structs.h"
#include "parser.h"
#include "external.h"

//handles external commands
void externalCommandHandler(INPUT* splitInput, CMDINFO* cmdinfo, FILED* fileD) {
    int pid = fork();
    if (pid == 0) { //child
        fdHandler(splitInput, cmdinfo, fileD);
        if (cmdinfo -> amp == 1) {
            printf("\n");//prevent background process from printing onto prompt line
        }
        if (execvp(splitInput -> command[0], splitInput -> command) == -1) {
            printf("Command \"%s\" not found.\n", splitInput -> command[0]);
        }
        //dont have to worry about closing files since child process

    } else { //parent
        if (cmdinfo -> amp == 1) { //background process
            ;//dont wait

        } else { //wait for child to complete if no '&'
            int status = 0;
            wait(&status);
        }
    }
}