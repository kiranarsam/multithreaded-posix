/**
 *  Two threads increment a shared counter, and a std::mutex is used to ensure that only
 *  one thread can access the critical section (incrementing the counter) at a time,
 *  preventing a race condition.
 */

#include <iostream>
#include <thread>
#include <mutex>

const int NUM_THREADS = 2;

// Shared data
int shared_counter = 0;

// Mutex for synchronization
std::mutex counter_mutex;

// Thread function
void increment_counter()
{
    for (int i = 0; i < 1000000; ++i)
    {
        // Lock the mutex before accessing the shared counter
        std::lock_guard<std::mutex> lock(counter_mutex);

        // Critical section: Increment the shared counter
        shared_counter++;
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
 * The std::mutex type is used to declare a mutex variable (counter_mutex).
 *
 * The std::lock_guard is used to lock (acquire) the mutex before entering the critical section.
 *  The std::lock_guard automatically releases the lock when it goes out of scope (at the end of
 *  the critical section).
 *
 * The critical section is the part of the code where shared resources (in this case,
 *  the shared_counter) are accessed. Access to this section is protected by the mutex to
 *  avoid race conditions.
 *
 * Two threads are created, and each increments the shared counter in a loop.
 *  The use of a mutex ensures that the counter is incremented atomically and avoids
 *  data corruption.
 */
