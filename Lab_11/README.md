# TCP Master-Worker PI Computation

## Overview
This lab distributes pi integration across separate worker processes connected over TCP. A master process partitions the interval range, sends work packets to workers, receives partial sums, and combines them into the final pi estimate.

## Exercise 1: Single-Threaded Workers
- `MasterWorkerTCP.c` listens on port `3000` and waits for the requested number of workers.
- The master divides the integration range into contiguous blocks.
- For each connected worker, a handler thread sends `start`, `end`, and step size `h`.
- `WorkerTCP.c` computes its assigned local sum and sends it back to the master.
- The master accumulates all worker results under a mutex and prints the final pi value.

## Exercise 2: Multithreaded Workers
- Keeps the same TCP master-worker protocol.
- The worker uses all online processors reported by `sysconf(_SC_NPROCESSORS_ONLN)`.
- Each worker subdivides its assigned block among local pthreads.
- Local thread sums are reduced inside the worker before sending one partial sum back to the master.
- Demonstrates two-level parallelism: distributed workers plus local multithreading.

## Build
```bash
gcc -pthread Exercise_1/MasterWorkerTCP.c -o master
gcc Exercise_1/WorkerTCP.c -o worker
gcc -pthread Exercise_2/MasterWorkerTCP.c -o master_mt
gcc -pthread Exercise_2/WorkerTCP.c -o worker_mt
```

Example flow:
```bash
./master 10000000 4
./worker 127.0.0.1 3000
```

Start as many worker processes as the master expects.
