/**
 * Race Condition
 *
 * Race conditions are often lead to unpredictable and unintended behaviour.
 *
 * A race condition is a phenomenon in concurrent programming where the behavior of
 *  a program depends on the relative timing of events, such as the order of execution of threads.
 *  It occurs when multiple threads or processes access shared data concurrently, and
 *  the final outcome of the program depends on the specific order in which their operations
 *  are executed.
 *
 * 1. Read-Modify-Write Operations:
 *      When multiple threads read a shared variable, modify its value, and then write the
 *  result back, a race condition may occur if the threads are not properly synchronized.
 *  For example, if two threads increment a counter concurrently, the final value might be
 *  incorrect due to the interleaving of operations.
 *
 * 2. Shared Resource Access:
 *      Accessing shared resources (e.g., files, databases, or critical sections of code) without
 *  proper synchronization can lead to race conditions. For instance, if two threads attempt to
 *  write to the same file simultaneously, the final content of the file may be unpredictable.
 *
 * 3. Thread Scheduling:
 *      The order in which threads are scheduled to run by the operating system can impact the
 *  outcome of a program. If the order of execution affects the program's logic, it may result in
 *  a race condition.
 *
 * Example:
 */

#include <stdio.h>
#include <pthread.h>

int shared_counter = 0;

void *increment_counter(void *arg)
{
    for (int i = 0; i < 1000000; ++i)
    {
        shared_counter++;
    }
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    // Create two threads
    pthread_create(&thread1, NULL, increment_counter, NULL);
    pthread_create(&thread2, NULL, increment_counter, NULL);

    // Wait for the threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // The expected result is 2000000, but due to the race condition, it might be different.
    printf("Final counter value: %d\n", shared_counter);

    return 0;
}

/**
 * In this example, two threads are incrementing a shared counter variable. Because the
 *  increment operation is not atomic, there is a race condition, and the final value of
 *  the counter is unpredictable.
 *
 * To address race conditions, synchronization mechanisms such as mutexes, semaphores,
 *  and other thread synchronization primitives are used to ensure that only one thread at a time
 *  can access shared resources or critical sections of code. By properly synchronizing access to
 *  shared data, race conditions can be avoided, and the program's behavior becomes more predictable.
 */
