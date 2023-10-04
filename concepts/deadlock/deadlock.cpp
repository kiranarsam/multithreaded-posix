/**
 * Deadlock:
 * Deadlock is a situation that can occur in a multi-threaded or multi-process environment
 * where two or more processes or threads are unable to proceed with their execution because
 * they are each waiting for a resource that the others possess.
 *
 * In the deadlock, the processes or threads are effectively stuck, and the system cannot
 * make any progress. Deadlocks are common in concurrent or distributed systems and can lead
 * to resource contention and system instability.
 *
 * There are 4 necessary conditions for a deadlock to occur.
 * 1. Mutual Exclusion:
 *      At least one resource must be held in a non-shareable mode, meaning that only one process
 * or thread can use it at a time.
 *
 * 2. Hold and Wait:
 *      A process or thread must hold at least one resource and be waiting to acquire additional
 * resources that are currently held by other processes or threads.
 *
 * 3. No Preemption:
 *      Resources cannot be forcibly taken away from a process or thread; they must be released
 * voluntairly.
 *
 * 4. Circulat Wait:
 *      A cycle of processes or threads exists such that each is waiting for a resource that is
 * held by another process or thread in the cycle.
 *
 *
 *
 * To prevent or resolve deadlocks, various strategies:
 * a. Resource Allocation Graphs
 * b. Locking and Synchronization Mechanisms
 * c. Timeout Mechanisms
 * d. Resource Manager and Deadlock Detection Algorithms
 * e. Prevention Strategies
 */

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg)
{
    pthread_mutex_lock(&mutex1);
    printf("Thread 1 acquired mutex1\n");

    // Introducing a delay to increase the likelihood of a deadlock
    sleep(1);

    pthread_mutex_lock(&mutex2);
    printf("Thread 1 acquired mutex2\n");

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

void *thread2(void *arg)
{
    pthread_mutex_lock(&mutex2);
    printf("Thread 2 acquired mutex2\n");

    // Introducing a delay to increase the likelihood of a deadlock
    sleep(1);

    pthread_mutex_lock(&mutex1);
    printf("Thread 2 acquired mutex1\n");

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    return NULL;
}

int main()
{
    pthread_t tid1, tid2;

    // Create threads
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    // Wait for threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}

// Techniques such as lock ordering, deadlock detection,
// and avoiding circular waits can be applied to prevent or mitigate deadlocks.

// CPP

// two threads try to acquire two locks in a different order, leading to a potential deadlock
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex1;
std::mutex mutex2;

void threadFunction1()
{
    std::lock_guard<std::mutex> lock1(mutex1);
    std::cout << "Thread 1 locked mutex1" << std::endl;

    // Introducing a delay to increase the likelihood of a deadlock
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::lock_guard<std::mutex> lock2(mutex2);
    std::cout << "Thread 1 locked mutex2" << std::endl;
}

void threadFunction2()
{
    std::lock_guard<std::mutex> lock2(mutex2);
    std::cout << "Thread 2 locked mutex2" << std::endl;

    // Introducing a delay to increase the likelihood of a deadlock
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::lock_guard<std::mutex> lock1(mutex1);
    std::cout << "Thread 2 locked mutex1" << std::endl;
}

int main()
{
    std::thread t1(threadFunction1);
    std::thread t2(threadFunction2);

    t1.join();
    t2.join();

    return 0;
}

// DEADLOCK using semaphore:
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore1, semaphore2;

void *thread1(void *arg)
{
    sem_wait(&semaphore1);
    printf("Thread 1 acquired semaphore1\n");

    // Introducing a delay to increase the likelihood of a deadlock
    sleep(1);

    // Thread 1 is holding semaphore1 and waiting for semaphore2
    sem_wait(&semaphore2);
    printf("Thread 1 acquired semaphore2\n");

    sem_post(&semaphore2);
    sem_post(&semaphore1);

    return NULL;
}

void *thread2(void *arg)
{
    sem_wait(&semaphore2);
    printf("Thread 2 acquired semaphore2\n");

    // Introducing a delay to increase the likelihood of a deadlock
    sleep(1);

    // Thread 2 is holding semaphore2 and waiting for semaphore1
    sem_wait(&semaphore1);
    printf("Thread 2 acquired semaphore1\n");

    sem_post(&semaphore1);
    sem_post(&semaphore2);

    return NULL;
}

int main()
{
    // Initialize semaphores
    sem_init(&semaphore1, 0, 1);
    sem_init(&semaphore2, 0, 1);

    pthread_t tid1, tid2;

    // Create threads
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    // Wait for threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Destroy semaphores
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);

    return 0;
}
