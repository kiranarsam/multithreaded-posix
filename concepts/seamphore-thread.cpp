
/**
 * In this example, two threads increment a shared counter.
 *  A std::mutex is used to protect the critical section, and a std::semaphore (available in C++20)
 *  is used to control access to the critical section by limiting the number of threads that
 *  can enter concurrently.
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>

const int NUM_THREADS = 2;
const int MAX_CONCURRENT_THREADS = 1; // Allow only one thread to enter the critical section at a time

// Shared data
int shared_counter = 0;

// Mutex for synchronization
std::mutex counter_mutex;

// Semaphore for controlling access to the critical section
std::counting_semaphore<MAX_CONCURRENT_THREADS> sem(1);

// Thread function
void increment_counter()
{
    for (int i = 0; i < 1000000; ++i)
    {
        // Wait on the semaphore to enter the critical section
        sem.acquire();

        // Lock the mutex before accessing the shared counter
        std::lock_guard<std::mutex> lock(counter_mutex);

        // Critical section: Increment the shared counter
        shared_counter++;

        // Release the semaphore to signal exit from the critical section
        sem.release();
    }
}

int main()
{
    std::thread threads[NUM_THREADS];

    // Create two threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i] = std::thread(increment_counter);
    }

    // Wait for the threads to finish
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i].join();
    }

    // Display the final value of the shared counter
    std::cout << "Final counter value: " << shared_counter << std::endl;

    return 0;
}

/**
 * std::counting_semaphore is a counting semaphore introduced in C++20.
 *  It is used to limit the number of threads that can enter the critical section
 *  to MAX_CONCURRENT_THREADS.
 *
 * sem.acquire() is used to wait on the semaphore, and
 * sem.release() is used to signal exit from the critical section.
 *
 * The std::mutex (counter_mutex) is still used to protect the critical section,
 *  ensuring that the shared counter is accessed atomically.
 *
 * g++ -o example example.cpp -std=c++20 -pthread
 */
