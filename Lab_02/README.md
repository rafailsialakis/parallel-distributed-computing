# Shared State and Race Conditions in C++

## Overview
This lab studies how multiple `std::thread` workers interact with shared arrays and counters. The programs intentionally use similar workloads with different ways of passing shared data, then introduce mutexes to remove data races.

## Exercise 1.1: Shared Data Passed as Arguments
- `SharedCounterArrayGlobal_1.1.cpp` starts four threads that increment shared array entries.
- `SharedCounterArrayGlobalWhile_1.1.cpp` uses a shared counter so each thread claims the next array position.
- Demonstrates how unsynchronized writes to shared memory can produce incorrect counts.
- Includes array-checking routines that report mismatches after all threads join.

## Exercise 1.2: Shared Struct Data
- Repeats the same two workloads using a shared `Data` struct.
- Groups the array, counter, thread count, and iteration limit into one object passed by pointer.
- Shows that reorganizing shared state does not remove race conditions by itself.

## Exercise 2: Mutex Protection
- Translates the shared-counter programs into global-state C++ versions.
- Uses `std::mutex` to protect critical sections.
- Compares locking the array-update loop with locking the shared counter update.
- Highlights the performance impact of lock placement and the deadlock risk when a mutex is not released on all paths.

## Build
```bash
g++ -std=c++11 -pthread Exercise_2/SharedCounterArrayGlobal.cpp -o shared_array
g++ -std=c++11 -pthread Exercise_2/SharedCounterArrayGlobalWhile.cpp -o shared_counter
```

Run the selected executable and inspect the reported error count.
