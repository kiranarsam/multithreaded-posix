/**
 * the term "atomic" refers to operations that are guaranteed to be executed without
 *  interruption by other threads. Atomic operations are fundamental for
 *  concurrent programming to avoid race conditions and ensure data integrity.
 * C++ provides atomic operations through the <atomic> header
 *  in the Standard Template Library (STL).
 */

#include <atomic>
#include <iostream>

std::atomic<int> counter(0);

int main()
{
    // Atomic increment
    counter.fetch_add(1);

    // Atomic load
    int current_value = counter.load();

    std::cout << "Current value: " << current_value << std::endl;

    return 0;
}

#include <atomic>
#include <iostream>

std::atomic<int> counter(0);

int main()
{
    // Atomic increment
    counter.fetch_add(1);

    // Atomic compare and exchange
    int expected = 1;
    int new_value = 2;
    bool success = counter.compare_exchange_strong(expected, new_value);

    if (success)
    {
        std::cout << "Exchange successful, new value: " << new_value << std::endl;
    }
    else
    {
        std::cout << "Exchange failed, current value: " << counter.load() << std::endl;
    }

    return 0;
}

#include <atomic>
#include <iostream>

std::atomic<int> counter(0);

int main()
{
    // Atomic increment with memory order
    counter.fetch_add(1, std::memory_order_relaxed);

    // Atomic load with memory order
    int current_value = counter.load(std::memory_order_relaxed);

    std::cout << "Current value: " << current_value << std::endl;

    return 0;
}
