# The Problem

The idea of this project is to create a discrete event simulation. Doing this provides an understanding of how processes move through a computing system, specifically through the CPU and disks. 

In this simulation, a job, which we will call an event, will begin at the CPU. After executing it will either be finished and let go from the system or it will go to one of two disks to perform I/O. 

Many of the decisions in the simulation are decided randomly, such as time spent at a device, which disk an event goes to if their queues are the same length, if an event is going to exit the system etc. 

## The Queues

There will be four queues needed for the simulation. A main priority queue will be used to keep track of the events. These events will be prioritized according to their logical clock time. 

> ### An aside: Clock Time
This simulation uses a "logical clock". This means time will be referred to as units and not an actual time (milliseconds). For example, an arbitrary event can start at time 15 and spend 7 units of time doing a particular task.

The other three queues are for the CPU and the two disks. These are FIFO queues, meaning the first event to enter the queue is the first one to be serviced by the queue's respective device. 

## The Mechanism 
- The simulation will have a start and finish time 
- Each iteration of the simulation loop, the unit of time will go up by one
- An initial event will be created and sent to the CPU
- When this and subsequent events are done at the CPU: 
    - The event will either exit the system or go to a disk for servicing based on a preset probability
    - A new event will be created with a randomly generated CPU start time and duration and placed in the event queue
- If the event does not exit the system, it will be placed in the shorter disk queue
- If this queue is empty, it will begin servicing immediately, if not it will be placed at the end of the queue and wait its turn
    - The time spent servicing will be determined randomly between two preset constants
- The loop will continue until the preset end time is reached
    - Any events currently in queues will be ignored and do not need to be processed. 

## Simulation Constants
- a SEED
    - used for the random number generator
    - with the same seed, we will get the same results for easy testing and debugging
- INIT_TIME
    - The start time of the simulation (time 0)
- FIN_TIME
    - The end time of the simulation 
- ARRIVE_MIN & ARRIVE_MAX
    - New processes will enter the system in a random interval between these two constants
- QUIT_PROB
    - The probability of an event leaving the system after executing on the CPU
- CPU_MIN & CPU_MAX
    - The time an event spends executing on the CPU will be a random time between these two constants
- DISK1_MIN & DISK1_MAX
    - The time an event spends doing I/O on disk1 will be a random time between these two constants
- DISK2_MIN & DISK2_MAX
    - Same as above, but with disk2

## Some Project Requirements

- Read the above mentioned simulation constants from a *text config file*
- The program should write to a *log file* the values of the simulation constants and each significant event
- Statistics to calculate and print
    - average and maximum size of each queue
    - utilization of each component (time busy/total time)
    - average and maximum response time of each component (response time is the difference in time of a job arrival and its completion at the component)
    - Throughput of each component (number of jobs completed per unit of time)
- The program should be run multiple times with different values for the simulation constants to achieve a good simulation
- The program should produce at least 1000 jobs
- inlcude a *RUNS.txt* file describing the data used to test the simulation and what has been learned
- choose a QUIT_PROB defaulted to 0.2


