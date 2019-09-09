//Joe Uzdzinski
//config source file

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void configConstants() {
    
    int config[12];

    FILE* fp = fopen("config.txt", "r");
    if (fp == NULL) {
        perror("error opening file");
        return;
    }

    char* line;
    char* token;

    int num = 0;
    int i = 0;

    while (fgets(line, 64, fp) != NULL) {
        token = strchr(line, ' ');
        num = atoi(token);
        config[i] = num;
        i++;
    }

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