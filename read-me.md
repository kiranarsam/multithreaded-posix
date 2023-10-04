
Data race is a common issue you may encounter in C++ concurrency and multi-threaded process.
Data races occur when at least two threads can simultaneously access a variable or memory location, and atleast one of those threads tries to access that variable.

This can result in undefined behaviour.
Concurrency is very important for handling multiple tasks at once.
Concurrent programming involves a shared memory location, and the different threads actually "read" the information provided by the previous threads.

Data race and race condition are not the same thing in the context of concurrent programming.
A data race happens when two threads access the same mutable object without synchronization, while a race condition happens when the order of events affects the correctness of the program.

A data race can cause a race condition, but not always. A race condition can also occur without a data race. Both are problems with atomicity and they can be solved by synchronization mechanisms like locks or serial queues.

Data Race
A data race occurs when one thread accesses a mutable object while another thread is writing to it, without any synchronization.

To avoid the data race, we can use a lock or a serial queue to synchronize the access to the counter.

Race Condition
A race condition occurs when the timing or order of events affects the correctness of a piece of code. A data race can cause a race condition, but not always.
Problem:
// A function that withdraws money from the balance
func withdraw(amount: Int) {
  // Check if there is enough balance
  if balance >= amount {
    // Simulate some delay
    Thread.sleep(forTimeInterval: 0.1)
    // Deduct the amount from the balance
    balance -= amount
  }
}

To avoid the race condition, we can use a lock or a serial queue to synchronize the access to the balance.
Solution:
// A function that withdraws money from the balance
func withdraw(amount: Int) {
  // Acquire the lock
  lock.lock()
  // Check if there is enough balance
  if balance >= amount {
    // Simulate some delay
    Thread.sleep(forTimeInterval: 0.1)
    // Deduct the amount from the balance
    balance -= amount
  }
  // Release the lock
  lock.unlock()
}