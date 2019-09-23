# My Solution

# Project Structure

## Input files: 
5 source (.c) files: 
- main.c
- event.c 
- queue.c 
- rng.c 
- config.c 

5 header (.h) files:
- structs.h
- event.h
- queue.h
- rng.h
- config.h

A config (.txt) file:
- config.txt

## Output files: 

2 (.txt) files:
- log.txt
- stats.txt

---
## Input 

### main.c 
Main.c is the center of the simulation. Before the simulation starts, some initialization steps are taken. First, the configuration is read in from config.txt. The SEED is set for the random number generator and then pointers for each queue are allocated memory. The queues used in the simulation are implemented by struct pointers. A NODE struct exists which will hold an EVENT pointer and a nextPtr, which points to the next NODE in the queue. More about the queues will be discussed with the queue.c source file. Next, COMPONENT struct pointers are initialized for each component of the simulation (CPU, DISK1, DISK2). These structs only hold one piece of information, which is the status of the respective component. Then, simulationStart(...) is called. This will add the first job event to the event queue (priority) and a SIMULATION_FINISH event that terminates the simulation at FIN_TIME. Next, a long list of variables are defined and initilized to zero. These variables are used for statistics that can be viewed at the end of the simulation. After all of these initialization steps, the main while loop begins. This loop controls the flow of the simulation. It starts by dequeueing the next event in the event queue. It reads the evnets type and sends it to its proper event handler using a switch statement. The simulation will not end until the SIMULATION_FINISH event is dequeued from the event queue. After the event is handled by the proper handler, each component is updated and set up for the next event to be dequeued.

### event.c 
Event.c consists of a createEvent(...) function, the simulationStart(...) function, all of the handler functions, the update component functions, and a printEventType(...) function which is only used for debugging. The handler and update functions are what move the events around the simulation. For example, the disk1FinishHandler(...) will set the disk1 status to IDLE to be able to accept another event for processing, set a variable for response time statistics, and then enqueue the finished event to the cpu queue for further processing. Each of these handlers work in a similar way.

### queue.c 
Queue.c is responsible for the queue functions. It contains an enqueue(...) function, dequeue(...) function, queueLength(...) function, and a printq(...) which is only used for debugging. The enqueue function is capable of adding an event into the priority queue or any of the 3 fifo queues. A pointer to the right queue, the event to be enqueued, and the type of queue (FIFO or PRIORITY) is passed to the function. The dequeue(...) function only takes a pointer to the queue, and removes/returns the foremost event in the queue. The queueLength(...) function only takes a pointer to the queue and returns the length of the queue. Printq(...) also only takes a pointer to the queue and prints the queue to the terminal. This is only used for debugging. 

### rng.c 
Rng.c only contains one function. This function, called randomNumberBetween(...) takes two integers as arguments and returns a random integer that lies between them.

### config.c
The config.c file does two things. The first thing it does is set the configuration. It reads in from config.txt the configuration constants and sets them equal to the proper constant. It's other functionality is printing information to the log.txt file. It does this with a function called printToLog(...) which is called right after dequeuing an event in the main simulation loop. printToLog(...) prints every event to the log.txt file in the following format: "At time 67, process 92 arrives at the CPU queue."

### structs.h
Structs.h contains 3 struct definitions (EVENT, NODE, and COMPONENT) and 2 enum definitions (eventType and status). The Event struct holds an event type, a job id, time, and three variables to keep track of component response times (I will explain this in the statistics section). The NODE and COMPONENT structs were explained in the main.c section above. The enums are for the event type of each event (PROCESS_ARRIVAL, DISK1_FINISH, etc.) and the status for the components (IDLE, BUSY). 

### event.h
Event.h only contains function declarations for event.c

### queue.h 
Queue.h contains function declarations for queue.c and an enum definition for type of queue (FIFO, PRIORITY). This enum is only used for the enqueue function.

### rng.h
Rng.h only contains a function declaration for rng.c

### config.h
Config.h contains function declarations for config.c and it declares each of the configuration constants as global variables. 

### config.txt
This file lets the user to update the simulation configuration constants without having to recompile the code.

## Output

### log.txt
When the simulation is run, this file will be updated in real time. If you open log.txt while its running you can see the events being printed. Before the logged events, you will see the configuration of the simulation. 

### stats.txt
When the SIMULATION_FINISH event is reached in the simulation, the simulation stats will be printed to this file. 

---
# Design Decisions

All functionality and info manipulation was meant to be removed from main.c to reduce cluttering and increase modularity. The event handlers do this well, taking functionality into event.c. Queue.c does a good job of removing a lot of clutter as well. One thing that I could not remove as easily were the statistics. A lot of the statistic calculations were done in main, and it adds a hefty chunk of code. It seemed unruly to add more parameters to the existing event handlers to calculate stats, so I left most of the stat calculation in main. One stat in particular proved to be difficult. The other stats you can easily see in the code how I solved them, but one is less clear. This stat was the max/average response time for each component. 
The only way I could think of doing it was by adding a few members to the EVENT struct. These members were a response time variable that would correlate to each component. I calculated response time, by first adding the currentTime to the respective variable when either leaving the CPU, leaving one of the DISKs, or when a new process arrives into the CPU queue. I would then subtract the currentTime when the event would make it through the queue (in the update component functions). This would result in a negative value, but its absolute value would equate to the proper response time. This was then checked to see if it was the max, then added to the total response time, and then reset to zero so the calculation could happen again on the processes next journey through a queue. Each time it was reset to zero, another variable which kept track of the components total iterations would be incremented by 1. This was used to calculate the average at the very end of the simulation. 
Other than that, everything else in the program is hopefully understandable with the aid of comments. 

---

# How to Run the Simulation
- download the above input files into a folder
- open terminal and cd to folder
- type 'make'
- project will be compiled and executable will be called main
- change the configuration inside "config.txt"
- type './main' to start simulation
- simulation finish will print to terminal when complete
- open "log.txt" to see simulation configuration and log 
- open "stats.txt" to see simulation statistics
