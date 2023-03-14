#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <native/task.h>
#include <native/timer.h>

// Task priority, for RT systems 99 is the highest
#define TASK_PRIORITY 99
// Sleep time in nanoseconds (equavcalent to 1ms)
#define TIMEOUT 1000000 
// How many times should the thread loop be executed
#define LOOPS 100 
void loop(void* arg) {
    int i = 0;
    struct timespec t;
    rt_task_set_periodic(NULL, TM_NOW, PERIOD_NS);
    while(1) {
        rt_task_wait_period(NULL);
        clock_gettime(CLOCK_MONOTONIC, &t);
        printf("Iteration %d: %ld.%09ld\n", i++, t.tv_sec, t.tv_nsec);
        // Do some computational work
        int a = 0;
        for (int j = 0; j < 1000000; j++) {
            a += j;
        }
    }
}

int main(int argc, char** argv) {

    // Create a real-time task
    RT_TASK task;

    if (rt_task_create(&task, "my_task", 0, TASK_PRIORITY, 0) != 0) {
        fprintf(stderr, "Error creating task!\n");
        exit(EXIT_FAILURE);
    }

    // Start the real-time task
    if (rt_task_start(&task, &loop, NULL) != 0) {
        fprintf(stderr, "Error starting task!\n");

        // Delete the task
        rt_task_delete(&task);
        exit(EXIT_FAILURE);
    }

    // Wait for the task to finish
    rt_task_join(&task);

    // Delete the task
    rt_task_delete(&task);
    return 0;
}
