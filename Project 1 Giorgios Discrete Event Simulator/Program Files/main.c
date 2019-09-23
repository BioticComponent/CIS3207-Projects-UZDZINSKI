//Joe Uzdzinski
//main source file
//this is a discrete event simulator, for further details, please see the README

#include "config.h"
#include "rng.h"
#include "queue.h"
#include "event.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

int currentTime = 0;

int main() {
    //initialize constants with values from config.txt file
    configConstants();

    //seed for random number generator
    srand(SEED);
    
    //allocating memory for start pointers for each queue
    NODE* EVENT_Qptr = malloc(sizeof(NODE));
    EVENT_Qptr -> nextPtr = NULL;
    NODE* CPU_Qptr = malloc(sizeof(NODE));
    CPU_Qptr -> nextPtr = NULL;
    NODE* DISK1_Qptr = malloc(sizeof(NODE));
    DISK1_Qptr -> nextPtr = NULL;
    NODE* DISK2_Qptr = malloc(sizeof(NODE));
    DISK2_Qptr -> nextPtr = NULL;

    //creating instance of component for each component
    //setting each components status to IDLE
    COMPONENT* CPU = malloc(sizeof(COMPONENT));
    CPU -> status = IDLE;
    COMPONENT* DISK1 = malloc(sizeof(COMPONENT));
    DISK1 -> status = IDLE;
    COMPONENT* DISK2 = malloc(sizeof(COMPONENT));
    DISK2 -> status = IDLE; 

    //testing for NULL to see if memory was allocated successfully for each pointer
    if (!EVENT_Qptr || !CPU_Qptr || !DISK1_Qptr || !DISK2_Qptr || !CPU || !DISK1 || !DISK2) {
        printf("Memory allocation failure.\n");
        return 0;
    }
    
    //start simulation by adding first job event and simulation termination event into event queue
    simulationStart(&EVENT_Qptr);

    //STATISTICS VARIABLES
    int jobsExited = 0;
    //hold queue length for each component
    int eventQLen = 0;
    int cpuQLen = 0;
    int d1QLen = 0;
    int d2QLen = 0;
    //hold max queue length for each component
    int max_eventQLen = 0;
    int max_cpuQLen = 0;
    int max_d1QLen = 0;
    int max_d2QLen = 0;
    //how many total events have been in each queue to find average for each iteration
    int eventQTotal = 0;
    int cpuQTotal = 0;
    int d1QTotal = 0;
    int d2QTotal = 0;
    //keep track of how many iterations the simulation goes through
    int iterations = 0;
    //hold time for total time busy
    int cpuBusyTotal = 0;
    int disk1BusyTotal = 0;
    int disk2BusyTotal = 0;
    //keep track of how many jobs each component processed 
    int cpuTotalJobs = 0;
    int disk1TotalJobs = 0;
    int disk2TotalJobs = 0;
    //keep track of response time for each queue
    int cpuResponseTotal = 0;
    int cpuMaxResponse = 0;
    int cpuIterations = 0;
    int disk1ResponseTotal = 0;
    int disk1MaxResponse = 0;
    int disk1Iterations = 0;
    int disk2ResponseTotal = 0;
    int disk2MaxResponse = 0;
    int disk2Iterations = 0;
    //holds how long each component is busy, determined at component_BEGIN
    //but only added to componentResponseTotal's if component finishes processing
    int cpuTempBusy = 0;
    int d1TempBusy = 0;
    int d2TempBusy = 0;

    //main while loop, this loop will run until SIMULATION_FINISH event at FIN_TIME
    int running = 1;
    while (running){

        //dequeue next event from event queue for handling
        EVENT* nextEvent = deq (&EVENT_Qptr);
        //prints event to log.txt
        printToLog(nextEvent);
        //update currentTime to nextEvent's time
        currentTime = nextEvent -> time;

        //STATISTICS:
        iterations++;
        eventQLen = queueLength(&EVENT_Qptr);
        eventQTotal += eventQLen;
        if (eventQLen > max_eventQLen) {
            max_eventQLen = eventQLen;
        }
        cpuQLen = queueLength(&CPU_Qptr);
        cpuQTotal += cpuQLen;
        if (cpuQLen > max_cpuQLen) {
            max_cpuQLen = cpuQLen;
        }
        d1QLen = queueLength(&DISK1_Qptr);
        d1QTotal += d1QLen;
        if (d1QLen > max_d1QLen) {
            max_d1QLen = d1QLen;
        }
        d2QLen = queueLength(&DISK2_Qptr);
        d2QTotal += d2QLen;
        if (d2QLen > max_d2QLen) {
            max_d2QLen = d2QLen;
        }
        
        //switch case will determine event type and send event to proper handler
        switch (nextEvent -> type) { 
            case PROCESS_ARRIVAL: {
                processArrivalHandler(&EVENT_Qptr, &CPU_Qptr, nextEvent);
                break;
            }
            case CPU_BEGIN: {
                cpuTempBusy = cpuBeginHandler(&EVENT_Qptr, nextEvent);
                break;
            }
            case CPU_FINISH: { 
                cpuFinishHandler(&EVENT_Qptr, nextEvent, &DISK1_Qptr, &DISK2_Qptr, CPU);
                
                //STATISTICS
                cpuTotalJobs++;
                //add response time and iteration to CPU stats (EXPLAINED IN README)
                cpuResponseTotal += nextEvent -> cpuQresponse;
                if (nextEvent -> cpuQresponse > cpuMaxResponse) {
                    cpuMaxResponse = nextEvent -> cpuQresponse;
                }
                cpuIterations++;
                // response time recorded, reset to 0 for next iteration
                nextEvent -> cpuQresponse = 0;
                //component is finished, so can add busy time to total 
                cpuBusyTotal += cpuTempBusy;
                break;
            }
            case PROCESS_EXIT: { 
                processExitHandler(nextEvent);

                //STATISTICS
                jobsExited++;
                break;
            }
            case DISK1_ARRIVAL: { 
                disk1ArrivalHandler(&DISK1_Qptr, nextEvent);
                break;
            }
            case DISK2_ARRIVAL: {
                disk1ArrivalHandler(&DISK2_Qptr, nextEvent);
                break;
            }
            case DISK1_BEGIN: { 
                d1TempBusy = disk1BeginHandler(&EVENT_Qptr, nextEvent);
                break;
            }
            case DISK2_BEGIN: {
                d2TempBusy = disk2BeginHandler(&EVENT_Qptr, nextEvent);
                break;
            }
            case DISK1_FINISH: { 
                disk1FinishHandler(&CPU_Qptr, nextEvent, DISK1);

                //STATISTICS
                disk1TotalJobs++;
                //add response time and iteration to DISK1 stats (EXPLAINED IN README)
                disk1ResponseTotal += nextEvent -> disk1Qresponse;
                if (nextEvent -> disk1Qresponse > disk1MaxResponse) {
                    disk1MaxResponse = nextEvent -> disk1Qresponse;
                }
                disk1Iterations++;
                //response time recorded, reset to 0 for next iteration
                nextEvent -> disk1Qresponse = 0;
                //component is finished, so can add busy time to total 
                disk1BusyTotal += d1TempBusy;
                break;
            }
            case DISK2_FINISH: { 
                disk2FinishHandler(&CPU_Qptr, nextEvent, DISK2);
                
                //STATISTICS
                disk2TotalJobs++;
                //add response time and iteration to DISK2 stats (EXPLAINED IN README)
                disk2ResponseTotal += nextEvent -> disk2Qresponse;
                if (nextEvent -> disk2Qresponse > disk2MaxResponse) {
                    disk2MaxResponse = nextEvent -> disk2Qresponse;
                }
                disk2Iterations++;
                //response time recorded, reset to 0 for next iteration
                nextEvent -> disk2Qresponse = 0;
                //component is finished, so can add busy time to total 
                disk2BusyTotal += d2TempBusy;
                break;
            }
            case SIMULATION_FINISH: { 

                printf("\nThe simulation has finished.\n");
                printf("Check log.txt for log of each event and the simulation configuration.\n");
                printf("Check stats.txt for some statistics related to the simulation.\n\n");

                //STATISTICS
                //simulation is terminated, calculate final stats and send to stats.txt file
                FILE* statPtr = fopen("stats.txt", "w");
                if (statPtr == NULL) {
                    perror("error opening stats file");
                    return 0;
                } 
                fprintf(statPtr, "Simulation Stats: \n\n");
                fprintf(statPtr, "jobs that have exited system:\t%d\n\n", jobsExited);
                fprintf(statPtr, "max event queue length:\t\t\t%d\n", max_eventQLen);
                fprintf(statPtr, "max cpu queue length:\t\t\t%d\n", max_cpuQLen);
                fprintf(statPtr, "max d1 queue length:\t\t\t%d\n", max_d1QLen);
                fprintf(statPtr, "max d2 queue length:\t\t\t%d\n", max_d2QLen);
                fprintf(statPtr, "average event queue length:\t\t%.2f\n", (double)eventQTotal/iterations);
                fprintf(statPtr, "average cpu queue length:\t\t%.2f\n", (double)cpuQTotal/iterations);
                fprintf(statPtr, "average d1 queue length:\t\t%.2f\n", (double)d1QTotal/iterations);
                fprintf(statPtr, "average d2 queue length:\t\t%.2f\n", (double)d2QTotal/iterations);
                fprintf(statPtr, "CPU utilization:\t\t\t\t%.2f\n", ((double)cpuBusyTotal)/(FIN_TIME-INIT_TIME));
                fprintf(statPtr, "disk1 utilization:\t\t\t\t%.2f\n", ((double)disk1BusyTotal)/(FIN_TIME-INIT_TIME));
                fprintf(statPtr, "disk2 utilization:\t\t\t\t%.2f\n", ((double)disk2BusyTotal)/(FIN_TIME-INIT_TIME));
                fprintf(statPtr, "CPU jobs per unit of time:\t\t%.2f\n", (double)cpuTotalJobs/(FIN_TIME-INIT_TIME));
                fprintf(statPtr, "disk1 jobs per unit of time:\t%.2f\n", (double)disk1TotalJobs/(FIN_TIME-INIT_TIME));
                fprintf(statPtr, "disk2 jobs per unit of time:\t%.2f\n", (double)disk2TotalJobs/(FIN_TIME-INIT_TIME));
                fprintf(statPtr, "max cpu response time:\t\t\t%d\n", cpuMaxResponse);
                fprintf(statPtr, "max disk1 response time:\t\t%d\n", disk1MaxResponse);
                fprintf(statPtr, "max disk2 response time:\t\t%d\n", disk2MaxResponse);
                fprintf(statPtr, "average cpu response time:\t\t%.2f\n", (double)cpuResponseTotal/cpuIterations);
                fprintf(statPtr, "average disk1 response time:\t%.2f\n", (double)disk1ResponseTotal/disk1Iterations);
                fprintf(statPtr, "average disk2 response time:\t%.2f\n", (double)disk2ResponseTotal/disk2Iterations);
                
                //close file
                fclose(statPtr);

                //set running to !running to end loop
                running = !running;
                break;
            }
        }
    // update CPU and Disk queues, creates new events where appropriate
    updateCPU(&EVENT_Qptr, &CPU_Qptr, CPU, currentTime); 
    updateDISK1(&EVENT_Qptr, &DISK1_Qptr, DISK1, currentTime);
    updateDISK2(&EVENT_Qptr, &DISK2_Qptr, DISK2, currentTime);
    }
    return 0;
}