#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

// Sleep time in nanoseconds (equavcalent to 1ms)
#define TIMEOUT 1000000 
// How many times should the thread loop be executed
#define LOOPS 100 

void *task(void *)
{
    struct timespec start_time, end_time, interval;

    // Set the timer interval
    interval.tv_sec = 0;
    interval.tv_nsec = TIMEOUT;

    // Record the start time
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    fprintf(stdout, "Starting task with 1ms period.\n");

    // Loop with timed periodically executed code
    for (int i = 0; i < LOOPS; i++) {
        // Sleep for the specified interval
        if(clock_nanosleep(CLOCK_MONOTONIC, 0, &interval, NULL) != 0) 
        {
            fprintf(stderr, "ERROR: Coudln't properly sleep the timer!\n");
            exit(EXIT_FAILURE);
        }

        // Do some computational work
        double x = 0;
        for (int j = 0; j < 100000; j++) {
            x = x + sin(j) * cos(i);
        }
    }

    // Record the end time
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Calculate the total time spent in the loop
    double time_spent = (end_time.tv_sec - start_time.tv_sec) +
                        (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;

    fprintf(stdout, "Time spent in loop: %f seconds\n", time_spent);

    return NULL;
}

int main(void)
{
    // Create thread indetified
    pthread_t thread;

    // Check if
    if (pthread_create(&thread, NULL, task, NULL) != 0)
    {
        fprintf(stderr, "ERROR: Thread failed to be created!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Thread successfully created!\n");

    // Wait for thread to exit
    if(pthread_join(thread, NULL) != 0) 
    {
        fprintf(stderr, "ERROR: Thread coudln't exit properly!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Thread successfully exited!\n");

    exit(EXIT_SUCCESS);
}

