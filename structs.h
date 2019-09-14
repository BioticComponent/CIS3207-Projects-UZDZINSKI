#ifndef STRUCTS_H
#define STRUCTS_H

//enum for types of events
typedef enum eventType {PROCESS_ARRIVAL, CPU_BEGIN, CPU_FINISH, PROCESS_EXIT, DISK1_ARRIVAL, DISK1_BEGIN, DISK1_FINISH, DISK2_ARRIVAL, DISK2_BEGIN, DISK2_FINISH, SIMULATION_FINISH} eventType;

//an event struct that holds type and time
typedef struct _EVENT {
    eventType type;
    int jobNo;
    int time; 
} EVENT;

//a NODE struct that holds a pointer to an EVENT stuct and a pointer that points to the next NODE
typedef struct _NODE {
    EVENT* eventPtr;
    struct _NODE* nextPtr;
} NODE;

//enum for component status
typedef enum status {IDLE, BUSY} status;

//component struct to keep track of cpu and disks status
typedef struct _COMPONENT {
    status status;
} COMPONENT;

#endif