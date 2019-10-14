//Joe Uzdzinski
//internal.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "structs.h"
#include "internal.h"


void internalCommandHandler(INPUT* splitInput, ENV* env, CMDINFO* cmdinfo) {
    
    //save command into variable
    char * intCommand = splitInput -> command[0];

    //internal commands

    //clr
    //clears screen
    if (strcmp(intCommand, "clr") == 0) {

        printf("\033[H\033[2J");


    //echo
    //prints back user command and arguments
    } else if (strcmp(intCommand, "echo") == 0) {
        int i = 1;
        while (splitInput -> command[i] != NULL) {
            printf("%s ", splitInput -> command[i]);
            i++;
        }
        printf("\n");


    //exit
    //exits shell
    } else if (strcmp(intCommand, "exit") == 0) {
        exit(0);


    //quit
    //exits shell, no different than "exit"
    } else if (strcmp(intCommand, "quit") == 0) {
        exit(0);


    //dir
    //prints the items in directory
    } else if (strcmp(intCommand, "dir") == 0) {
        //allocate memory for potential cwd
        char * buffer = malloc(sizeof(char)*200);

        //remember starting directory, so we can return after reading contents 
        // of other directories
        char * tempDir = malloc(sizeof(char)*200);
        strcpy(tempDir, env -> currDir);
        
        int success = 1;

        if (splitInput -> command[1]) {//if command has argument
            if (chdir(splitInput -> command[1]) == 0) {
                
                getcwd(buffer, sizeof(char)*200);
                env -> currDir = buffer;
                
            } else {
                printf("directory <%s> not found\n", splitInput -> command[1]);
                success = 0;
            }
        }

        DIR *dirp; //directory stream pointer
        struct dirent *dentp; //directory entry pointer
        dirp = opendir(env -> currDir);  //open directory and set pointer 
        
        if (dirp && success) { //if found directory
            while ((dentp=readdir(dirp))) { //while new directory entry exists
                //dont include "." and ".." directories in list
                if(dentp -> d_name[0] != '.') {
                    printf("%s", dentp -> d_name);
                    printf("\n");
                }   
            }
        }
        
        closedir(dirp);
        
        //if there was a valid <directory> argument, change directory back
        if (splitInput -> command[1] && success) {

            //change back to starting directory
            chdir(tempDir);
            
            getcwd(buffer, sizeof(char)*200);
            env -> currDir = buffer;
            
            free(tempDir);
        }


    //pause
    //pauses the shell until 'enter' is pressed
    } else if (strcmp(intCommand, "pause") == 0) {
        puts("MyShell is paused until you press 'enter' ");
        
        getchar();//"absorbs" newline
        getchar();//waits for enter pressed

    
    //environ
    //prints environment variables
    } else if (strcmp(intCommand, "environ") == 0) {
        printf("Shell Directory: %s\n", env -> shellDir);
        printf("Current Directory: %s\n", env -> currDir);
        // return 0;


    //cd <directory>
    //changes directory to <directory>
    } else if (strcmp(intCommand, "cd") == 0) {
        char * buffer = malloc(sizeof(char)*200);
        if (splitInput -> command[1]) {
            if (chdir(splitInput -> command[1]) == 0) {
                getcwd(buffer, sizeof(char)*200);
                env -> currDir = buffer;
            } else {
                printf("directory <%s> not found.\n", splitInput -> command[1]);
            }
        } else {
            puts("You must provide a directory.");
        }

    //help
    } else {
        FILE* helpPtr = fopen ("readme", "r");
        char * nextLine = malloc(sizeof(char)*100);
        if (helpPtr == NULL) {
            puts("Unable to read help, continue\n");
            fclose(helpPtr);
        }
        while (fgets(nextLine, 100, helpPtr) != NULL) {
            printf("%s", nextLine);
        }
        fclose(helpPtr);
        free(nextLine);
    }

}

int checkInternal (INPUT* splitInput) {

    char * command = splitInput -> command[0];

    if ( (strcmp(command, "clr") == 0) || (strcmp(command, "echo") == 0) || (strcmp(command, "exit") == 0) || (strcmp(command, "quit") == 0) || (strcmp(command, "dir") == 0) || (strcmp(command, "pause") == 0) || (strcmp(command, "environ") == 0) || (strcmp(command, "cd") == 0) || (strcmp(command, "help") == 0)) {
        return 1;
    }

    return 0;
}