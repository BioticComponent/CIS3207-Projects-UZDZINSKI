//Joe Uzdzinski

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum eventType {PROCESS_ARRIVAL, CPU_ENTER, PROCESS_FINISH, PROCESS_EXIT, DISK1_ARRIVAL, DISK1_FINISH, DISK2_ARRIVAL, DISK2_FINISH, SIMULATION_FINISH} eventType;
int currentTime = 0;


int main() {

    int running = 1;
    while (!eventQueue.empty() && running){ // main simulation loop (runs until hit SIMULATION_FINISH event)
        
        currentTime = eventQueue.top().time; // update currentTime of simulation
        
        switch (eventQueue.top().eventType) { // switch case to determine how to handle event
            case PROCESS_ARRIVAL: 
                handle_process_arrival();
            case CPU_ENTER: 
                eventQueue.pop();
            case PROCESS_FINISH: 
                handle_process_finish);
            case PROCESS_EXIT: 
                eventQueue.pop();
            case DISK1_ARRIVAL: 
                eventQueue.pop();
            case DISK2_ARRIVAL:
                eventQueue.pop();
            case DISK1_FINISH: 
                handle_disk_finish();
            case DISK2_FINISH: 
                handle_disk_finish();
            case SIMULATION_FINISH: 
                running = !running;
    }
    // update CPU and Disk queues, creates new events where appropriate
    update_CPU(); 
    update_disks();
}