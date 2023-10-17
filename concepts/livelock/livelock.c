/**
 * Livelock occurs when two or more processes keep changing their state in response to
 *  the actions of the other, without making any progress.
 *
 * Below is an example in C that demonstrates a livelock using semaphores.
 * In this example, two threads are trying to share a resource,
 *  but the communication between them leads to a livelock situation:
 *
 * A livelock is a situation where two or more threads are unable to proceed because
 *  each is trying to release a resource that the other is trying to acquire, resulting in
 *  a continuous loop of resource contention without making progress.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;

void *threadA(void *arg)
{
    while (1)
    {
        printf("Thread A is trying to acquire the semaphore\n");
        sem_wait(&semaphore);

        // Perform some work
        printf("Thread A acquired the semaphore and is doing some work\n");
        sleep(1);

        printf("Thread A is releasing the semaphore\n");
        sem_post(&semaphore);
    }

    pthread_exit(NULL);
}

void *threadB(void *arg)
{
    while (1)
    {
        printf("Thread B is trying to acquire the semaphore\n");
        sem_wait(&semaphore);

        // Perform some work
        printf("Thread B acquired the semaphore and is doing some work\n");
        sleep(1);

        printf("Thread B is releasing the semaphore\n");
        sem_post(&semaphore);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t tidA, tidB;

    // Initialize semaphore
    sem_init(&semaphore, 0, 1);

    // Create threads
    pthread_create(&tidA, NULL, threadA, NULL);
    pthread_create(&tidB, NULL, threadB, NULL);

    // Wait for threads to finish
    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);

    // Destroy semaphore
    sem_destroy(&semaphore);

    return 0;
}

/**
 * In this example:
 *
 * Two threads (threadA and threadB) are created using pthread_create.
 * A semaphore (semaphore) is used for synchronization, initialized with a count of 1.
 * Both threads continuously try to acquire the semaphore,
 *  perform some work, and then release the semaphore.
 *
 * However, because of the continuous and identical attempts to acquire the semaphore,
 *  they end up in a livelock situation.
 * In a livelock, threads are actively trying to resolve a situation,
 *  but they do so in a way that doesn't make any progress. They remain stuck in
 *  a loop of actions without reaching a resolution.
 */

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex1;
std::mutex mutex2;

void threadFunction1()
{
    while (true)
    {
        std::cout << "Thread 1 trying to acquire mutex1\n";
        std::unique_lock<std::mutex> lock1(mutex1);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work

        std::cout << "Thread 1 trying to acquire mutex2\n";
        std::unique_lock<std::mutex> lock2(mutex2);

        // Release both locks to avoid deadlock
        lock2.unlock();
        lock1.unlock();
    }
}

void threadFunction2()
{
    while (true)
    {
        std::cout << "Thread 2 trying to acquire mutex2\n";
        std::unique_lock<std::mutex> lock2(mutex2);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work

        std::cout << "Thread 2 trying to acquire mutex1\n";
        std::unique_lock<std::mutex> lock1(mutex1);

        // Release both locks to avoid deadlock
        lock1.unlock();
        lock2.unlock();
    }
}

int main()
{
    std::thread thread1(threadFunction1);
    std::thread thread2(threadFunction2);

    thread1.join();
    thread2.join();

    return 0;
}
