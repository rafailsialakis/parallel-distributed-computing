# Recursive Parallelism

## Overview
This lab explores divide-and-conquer parallelism with POSIX threads. The programs recursively split work until a threshold is reached, then solve the smaller subproblems sequentially.

## Exercise 1: Recursive PI Calculation
- `piDivAndConq.c` approximates pi using recursive range splitting.
- For large ranges, it creates both left and right child threads.
- When the workload falls below the recursion limit, it performs the integration loop directly.
- Combines the left and right partial sums after joining both child threads.

## Exercise 2: Half-Threaded Recursive PI Calculation
- `piDivAndConqHalf.c` uses a lighter recursive strategy.
- One half of the split is handled in a new thread while the other half is computed in the current thread.
- Reduces thread creation overhead compared with spawning two child threads at every split.
- Uses the same command-line arguments and pi integration formula as Exercise 1.

## Exercise 3: Recursive Merge Sort
- `mergesort.c` adapts merge sort to a threaded divide-and-conquer structure.
- Splits the array recursively and creates pthread workers for subranges.
- Merges sorted halves with the standard merge routine.
- Uses a small built-in input array for demonstration.

## Build and Run
```bash
gcc -pthread Exercise_1/piDivAndConq.c -lm -o pi_div_conq
gcc -pthread Exercise_2/piDivAndConqHalf.c -lm -o pi_div_conq_half
gcc -pthread Exercise_3/mergesort.c -o mergesort
```

Example:
```bash
./pi_div_conq 10000000 1000000
```
