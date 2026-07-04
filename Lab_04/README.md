# Producer-Consumer Patterns and Monitors

## Overview
This lab implements coordination patterns for threads that wait on shared state. The exercises use semaphores, condition variables, a thread-safe queue, and monitor-style encapsulation.

## Exercise 1: Producer-Consumer Synchronization
- `Semaphore.cpp` coordinates one producer and one consumer with `std::binary_semaphore`.
- `ConditionalVar.cpp` implements the same producer-consumer flow with `std::condition_variable`.
- The producer periodically increments a shared item count.
- Consumers block until an item is available, consume it, and update the shared state.

## Exercise 2: Thread-Safe Queue
- Implements a templated `TSQueue<T>` backed by `std::queue`.
- Uses a mutex and condition variable so consumers block on `pop()` until data exists.
- Passes `Message` objects between producer and consumer threads.
- Demonstrates a cleaner producer-consumer design that hides synchronization inside the queue abstraction.

## Exercise 3: Bounded Parking Simulation
- Simulates ten cars repeatedly arriving, parking, and departing from a parking lot with four spaces.
- `Semaphore.cpp` uses a counting semaphore for capacity and a binary semaphore for mutual exclusion.
- `Conditional.cpp` uses a condition variable that waits for free spaces.
- `Monitor.cpp` encapsulates the condition variable, mutex, and parking-space state inside one class.

## Build
```bash
g++ -std=c++20 -pthread Exercise_1/Semaphore.cpp -o producer_consumer_sem
g++ -std=c++20 -pthread Exercise_3/Monitor.cpp -o parking_monitor
```

The producer-consumer programs run continuously; stop them manually after observing the output.
