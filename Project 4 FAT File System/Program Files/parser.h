//Joe Uzdzinski
//parser.h

#ifndef PARSER_H
#define PARSER_H

int getParsedCommand(char** argsPtr, ENV* env, CMDINFO* cmdinfo);
int getBatchCommand(char * batchCommand, char ** argsPtr, ENV* env, CMDINFO* cmdinfo);
void printArray(char** arrPtr);
void nullifyArray(char** arrPtr);
void splitInputHandler (char** argsPtr, INPUT* splitInput, CMDINFO* cmdinfo);
void fdHandler (INPUT* splitInput, CMDINFO* cmdinfo, FILED* fileD);
void pipeHandler(INPUT* splitInput, ENV* env, CMDINFO* cmdinfo, FILED* fileD);


#endif