# Multithreading Experiments in C++

## Overview
This project contains two small C++ programs demonstrating basic multithreading behavior using `std::thread`. The focus is on understanding concurrency and non-deterministic output when multiple threads access shared output streams.

## Program 1: Basic Multithreading
- Creates 10 threads
- Each thread prints a multiplication table for `(i + 1)`
- Demonstrates **non-deterministic output order** due to concurrent execution
- No synchronization mechanisms are used

## Program 2: Thread Scheduling & Sleep Behavior
- Same structure as Program 1
- Introduces `std::this_thread::sleep_for` to delay threads (except thread 0)
- Shows how thread scheduling affects execution order
- Demonstrates how delaying threads can temporarily reduce output interleaving, but does not eliminate non-determinism

## Build
```bash
g++ -std=c++11 main.cpp -o program -pthread
