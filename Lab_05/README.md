# Static Data Parallelism

## Overview
This lab applies static workload partitioning to numeric, image-processing, and Boolean satisfiability workloads. Each program divides the input range into blocks and assigns those blocks to threads.

## Exercise 1: Vector and Matrix Addition
- `VectorAdd.cpp` initializes three vectors and computes `a[i] = b[i] + c[i]`.
- `MatrixAdd.cpp` performs row-blocked matrix addition.
- Uses `std::thread::hardware_concurrency()` to choose the thread count.
- Measures elapsed execution time with `std::chrono`.

## Exercise 2: Parallel Grayscale Conversion
- `GrayScaleParallel.java` reads an image, splits its rows across worker threads, and writes a grayscale image.
- Applies the weighted RGB formula using coefficients for red, green, and blue.
- Accepts input file, output file, and thread count from the command line.
- Includes timing notes for several images and core counts.

## Exercise 3: Parallel SAT Search
- `SatParallel.java` checks all bit-vector assignments for a hard-coded Boolean circuit.
- Splits the `2^size` search space into contiguous blocks.
- Stores satisfying assignments in a synchronized output list.
- Reports all satisfying assignments and total runtime.

## Build and Run
```bash
g++ -std=c++11 -pthread Exercise_1/VectorAdd.cpp -o vector_add
g++ -std=c++11 -pthread Exercise_1/MatrixAdd.cpp -o matrix_add
javac Exercise_2/GrayScaleParallel.java
javac Exercise_3/SatParallel.java
```

Example Java runs:
```bash
java -cp Exercise_2 GrayScaleParallel input.jpg output.jpg 4
java -cp Exercise_3 SatParallel 23 4
```
