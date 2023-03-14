#include <sys/mman.h>
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <math.h>


// Task priority, for RT systems 99 is the highest
#define TASK_PRIORITY 99
//Clock resolution is 1 ns by default
#define CLOCK_RES 1e-9 
//Expressed in ticks 1 ms
#define LOOP_PERIOD 1e6 
// How many times should the thread loop be executed
#define LOOPS 100 

void loop(void *arg)
{
    RTIME tstart, now;

    //Print the info
    printf("Starting task %s with period of 1 ms ....\n");

    //Make the task periodic with a specified loop period
    if(rt_task_set_periodic(NULL, TM_NOW, LOOP_PERIOD) != 0) 
    {
        fprintf(stderr, "ERROR: Coudln't set the period for the timer!\n");
        exit(EXIT_FAILURE);
    }

    // Save start time
    tstart = rt_timer_read();

    //Start the task loop
    for(int i = 0; i < LOOPS; ++i)
    {
        printf("Loop count: %d, Loop time: %.5f ms\n", ctr, (rt_timer_read() - tstart)/1000000.0);
        ctr++;
        rt_task_wait_period(NULL);
    }
}


int main(int argc, char **argv)
{
    // Create a real-time task
    RT_TASK rtask;

    // Create task
    if (rt_task_create(&rtask, "my_task", 0, TASK_PRIORITY, 0) != 0) {
        fprintf(stderr, "ERROR: Couldn't create task!\n");
        exit(EXIT_FAILURE);
    }

    // Start the task
    if (rt_task_start(&rtask, &loop, NULL) != 0) {
        fprintf(stderr, "ERROR: Couldn't start task!\n");

        // Delete the task
        rt_task_delete(&rtask);
        exit(EXIT_FAILURE);
    }

    // Wait for the task to finish
    if(rt_task_join(&rtask) == 0)
    {
        frpintf(stderr, "ERROR: Couldn't exit properly!\n")
        exit(EXIT_FAILURE);
    }

    // Delete the task
    rt_task_delete(&rtask);

    exit(EXIT_SUCCESS);
}