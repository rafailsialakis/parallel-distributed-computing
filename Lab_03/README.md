# Mutual Exclusion Techniques and Dining Philosophers

## Overview
This lab extends the shared-counter experiments with higher-level synchronization patterns. It compares a Java-style `synchronized` wrapper, fine-grained locking, atomic operations, and a classical dining philosophers solution.

## Exercise 1: Synchronized-Style Critical Sections
- Reworks the shared array and shared counter examples with `std::recursive_mutex`.
- Defines a `synchronized(mutex)` macro using `std::unique_lock`.
- Protects critical sections while preserving the structure of the original threaded loops.
- Demonstrates how scoped locking can make lock/unlock lifetime clearer.

## Exercise 2: Fine-Grained Locking and Atomics
- `SharedCounterArrayGlobalFineGrained.cpp` assigns a separate mutex to each array element.
- `SharedCounterArrayGlobalWhileFineGrained.cpp` experiments with an array of recursive mutexes.
- `AtomicArray.cpp` replaces explicit locks with `std::atomic<int>` and `fetch_add`.
- Shows the tradeoff between coarse locks, per-element locks, and lock-free atomic updates.

## Exercise 3: Dining Philosophers
- Models five philosophers and five forks using classes.
- Each fork owns a mutex and exposes `get()` / `put()` operations.
- A global waiter mutex serializes fork acquisition to avoid deadlock.
- Philosopher threads repeatedly think, acquire forks, eat, and release forks.

## Build
```bash
g++ -std=c++17 -pthread Exercise_3/DiningTiming.cpp -o dining
g++ -std=c++11 -pthread Exercise_2/AtomicArray.cpp -o atomic_array
```

The dining philosophers program is intentionally continuous and should be stopped manually.
