# Reductions and MapReduce-Style Parallelism

## Overview
This lab focuses on reducing partial results from parallel workers. It compares lock placement in numeric integration with map/reduce-style implementations for string matching and SAT search.

## Exercise 1: Parallel PI Integration
- Approximates pi with numerical integration of `4 / (1 + x^2)`.
- `VectorSumNoLock.cpp` stores one local sum per thread and combines after joining.
- `IntegralLocks.cpp` uses a local sum inside each thread and locks only while adding to the shared sum.
- `BadMutualExclusion.cpp` locks around every iteration, demonstrating excessive synchronization overhead.

## Exercise 2: Pattern Matching
- `MapReducePatternMatching.java` reads a file into memory and searches for a pattern.
- Worker threads scan separate ranges of possible match offsets.
- A shared `match` array marks positions where the pattern occurs.
- Reports elapsed time and matching positions.

## Exercise 3: SAT MapReduce
- `SATMapReduce.java` partitions the Boolean assignment space by thread.
- Each worker writes satisfying assignments to a private list.
- The main thread reduces the per-thread lists into one final output.
- Avoids the synchronized shared list used in the earlier SAT implementation.

## Build and Run
```bash
g++ -std=c++11 -pthread Exercise_1/VectorSumNoLock.cpp -o pi_no_lock
g++ -std=c++11 -pthread Exercise_1/IntegralLocks.cpp -o pi_locks
javac Exercise_2/MapReducePatternMatching.java
javac Exercise_3/SATMapReduce.java
```

Example runs:
```bash
./pi_no_lock 10000000 4
java -cp Exercise_2 MapReducePatternMatching input.txt pattern
java -cp Exercise_3 SATMapReduce 23 4
```
