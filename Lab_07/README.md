# Pthreads Text Processing and Prime Sieve Scheduling

## Overview
This lab uses POSIX threads for larger workloads. It implements a parallel word counter and compares block, cyclic, and dynamic scheduling strategies for the Sieve of Eratosthenes.

## Exercise 1: Parallel Word Count
- `wordcount.c` reads a full text file into memory.
- Splits the file into chunks, adjusting chunk boundaries so words are not cut in half.
- Each thread tokenizes its chunk and stores word counts in a local binary search tree.
- The main thread merges all partial trees and prints the final counts.
- Includes timing notes for running on repeated copies of `bible.txt`.

## Exercise 2: Parallel Prime Counting
- `SieveBlock.c` assigns contiguous ranges to worker threads.
- `SieveCyclic.c` assigns candidates cyclically by thread index.
- `SieveDynamic.c` uses a shared job counter protected by a mutex so threads claim chunks dynamically.
- All versions first compute base primes up to `sqrt(size)` and then mark composites in the remaining range.
- Each program prints the number of primes found and elapsed time.

## Build
```bash
gcc -pthread Exercise_1/wordcount.c -o wordcount
gcc -pthread Exercise_2/SieveBlock.c -lm -o sieve_block
gcc -pthread Exercise_2/SieveCyclic.c -lm -o sieve_cyclic
gcc -pthread Exercise_2/SieveDynamic.c -lm -o sieve_dynamic
```

Example runs:
```bash
./wordcount Exercise_1/bible.txt 4
./sieve_dynamic 100000000 4
```
