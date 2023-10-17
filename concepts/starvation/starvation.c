/**
 * Starvation:
 *
 * Starvation is a situation that can occur in concurrent systems when a process or a thread is
 *  unable to make progress because it is unable to acquire the necessary resources or
 *  is constantly being preempted by other higher-priority tasks.
 *
 * In the context of concurrent programming, 'starvation' refers to the condition where
 * a process or thread is perpetually denied access to a resource it needs,
 *  preventing it from completing its execution.
 *
 * 1. Resource Starvation:
 *      Occurs when a process or thread is unable to acquire a resource it needs to execute.
 *  This can be due to competition for limited resources, and some processes might acquire
 *  the resource repeatedly, leaving others waiting indefinitely.
 *
 * 2. Priority Inversion:
 *      In systems where priority scheduling is used, priority inversion can occur.
 *  This happens when a high-priority task is waiting for a resource currently held by
 *  a low-priority task. If the low-priority task does not release the resource promptly,
 *  the high-priority task may be delayed indefinitely.
 *
 * 3. Aging:
 *      Aging is a concept in priority scheduling where the priority of a task increases over time,
 *  even if it does not get access to the CPU. Without aging, lower-priority tasks might starve,
 *  never getting a chance to execute.
 */

// Example:
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
int shared_resource = 0;

void *low_priority_thread(void *arg)
{
    pthread_mutex_lock(&mutex);

    // Critical section
    printf("Low priority thread entering critical section\n");
    shared_resource++;
    sleep(2); // Simulate some work
    printf("Low priority thread leaving critical section\n");

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *high_priority_thread(void *arg)
{
    // Simulate some work
    sleep(1);

    pthread_mutex_lock(&mutex);

    // Critical section
    printf("High priority thread entering critical section\n");
    shared_resource++;
    printf("High priority thread leaving critical section\n");

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t low_priority, high_priority;

    pthread_mutex_init(&mutex, NULL);

    // Create two threads with different priorities
    pthread_create(&low_priority, NULL, low_priority_thread, NULL);
    pthread_create(&high_priority, NULL, high_priority_thread, NULL);

    // Wait for the threads to finish
    pthread_join(low_priority, NULL);
    pthread_join(high_priority, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}

/**
 * In this example:
 *
 * Two threads are created, one with low priority and the other with high priority.
 * The high-priority thread is delayed by the low-priority thread, which holds a mutex
 *  in its critical section.
 * As a result, the high-priority thread is starved and cannot execute in a timely manner.
 */

#include <iostream>
#include <thread>
#include <mutex>

std::mutex commonMutex;

void lowPriorityTask()
{
    std::lock_guard<std::mutex> lock(commonMutex);

    std::cout << "Low-priority task is executing.\n";
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void highPriorityTask()
{
    std::cout << "High-priority task is waiting for the mutex.\n";

    std::lock_guard<std::mutex> lock(commonMutex);

    std::cout << "High-priority task acquired the mutex and is executing.\n";
}

int main()
{
    std::thread lowPriorityThread(lowPriorityTask);
    std::thread highPriorityThread(highPriorityTask);

    lowPriorityThread.join();
    highPriorityThread.join();

    return 0;
}

/**
 * In this example, the high-priority task is waiting for a mutex that the low-priority
 *  task is holding. Due to the priority inversion, the low-priority task may continue
 *  to execute, preventing the high-priority task from making progress,
 *  potentially leading to starvation.
 */
