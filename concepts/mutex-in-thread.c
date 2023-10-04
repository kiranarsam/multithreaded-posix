/**
 * In multithreaded programming, a mutex (short for mutual exclusion) is commonly used to
 * protect shared resources and ensure that only one thread can access the critical section at a time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

int shared_data = 0;   // Shared resource
pthread_mutex_t mutex; // Mutex for synchronization

void *threadFunction(void *threadID)
{
    long tid = (long)threadID;

    for (int i = 0; i < 3; ++i)
    {
        // Lock the mutex before accessing the shared resource
        pthread_mutex_lock(&mutex);

        // Critical section: Access the shared resource
        printf("Thread %ld: Shared data before increment: %d\n", tid, shared_data);
        shared_data++;
        printf("Thread %ld: Shared data after increment: %d\n", tid, shared_data);

        // Unlock the mutex after accessing the shared resource
        pthread_mutex_unlock(&mutex);

        // Simulate some work in non-critical section
        usleep(100000);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("Main: Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, threadFunction, (void *)t);

        if (rc)
        {
            printf("Error: return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    printf("Main: Shared data after all threads: %d\n", shared_data);

    pthread_exit(NULL);
}

/**
 * In this example:
 *
 * The shared resource is the variable 'shared_data'.
 * The 'pthread_mutex_t' type is used to represent the mutex.
 * Each thread runs the 'threadFunction', which increments the shared resource inside a
 *  critical section protected by the mutex.
 * The 'pthread_mutex_lock' function is used to lock the mutex before entering the critical section,
 *  and 'pthread_mutex_unlock' is used to release the mutex after exiting the critical section.
 */

/**
 * gcc -o multithread_mutex multithread_mutex.c -pthread
 */
