/* | Threads | 1           | 2           | 4           | 8
 * | Time    | 2.79495 sec | 1.87883 sec | 1.32614 sec | 1.20855 sec
 * |   
 * | Number = 1 billion
 * | CPU Intel(R) Core(TM) i5-1035G1 CPU @ 1.00GHz
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

struct threadargs{
    int start;
    int end;
    int* base_primes;
    int size;
    bool* prime;
};
struct timespec start, finish;
void* sieve(void*);
int main(int argc, char *argv[])
{
    int size = 0;
    int numofthreads = 0;
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        printf("Usage: %s <size> <threads>\n", argv[0]);
        return 1;
    }
    
    // Convert argument to integer
    char *endptr1;
    size = strtol(argv[1], &endptr1, 10);
    
    // Check if the conversion was successful
    if (*endptr1 != '\0') {
        printf("Integer argument expected\n");
        return 1;
    }
    if (argc != 3) {
        printf("Usage: %s <size> <threads>\n", argv[0]);
        return 1;
    }
    // Convert argument to integer
    char *endptr2;
    
    numofthreads = strtol(argv[2], &endptr2, 10);
    
    // Check if the conversion was successful
    if (*endptr2 != '\0') {
        printf("Integer argument expected\n");
        return 1;
    }
    
    // Check if size is positive
    if (size <= 0) {
        printf("size should be positive integer\n");
        return 1;
    }
    
    // Allocate memory for the prime array
    bool *prime = (bool*)malloc((size + 1) * sizeof(bool));
    if (prime == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Initialize all entries as true (potentially prime)
    for (int i = 2; i <= size; i++) {
        prime[i] = true;
    }
    
    // Set 0 and 1 as not prime
    if (size >= 0) prime[0] = false;
    if (size >= 1) prime[1] = false;
    
    // Get current time
    clock_gettime(CLOCK_MONOTONIC, &start);
    // Apply Sieve of Eratosthenes algorithm
    
    int limit = (int)sqrt(size) + 1;
    for (int p = 2; p <= limit; p++) {
        // If prime[p] is true, then it is a prime
        if (prime[p]) {
            // Update all multiples of p
            for (int i = p * p; i <= limit; i += p) {
                prime[i] = false;
            }
        }
    }
    int count = 0;
    int* base_primes;
    for (int i = 2; i <= limit; i++) {
        if (prime[i]) {
            //printf("%d\n", i); // Uncomment to print all primes
            count++;
        }
    }
    int index = 0;
    base_primes = (int*) malloc(sizeof(int)*count);
    for(int i = 2; i <= limit; i++){
        if(prime[i]){
            base_primes[index] = i;
            index++;
        }
    }
    struct threadargs* args;
    pthread_t* threads;
    args = (struct threadargs*) malloc(sizeof(struct threadargs) * numofthreads);
    threads = (pthread_t*) malloc(sizeof(pthread_t) * numofthreads);
    int block_size = (size - limit)/numofthreads;
    for(int i = 0; i < numofthreads; i++){
        int thread_start = limit + 1 + i * block_size;
        int thread_end = (i == numofthreads - 1) ? size : thread_start + block_size - 1;
        args[i].start = thread_start;
        args[i].end = thread_end;
        args[i].prime = prime;
        args[i].size = count;
        args[i].base_primes = base_primes;
    }
    for(int i = 0; i < numofthreads; i++){
        pthread_create(&threads[i], NULL, sieve, &args[i]);
    }
    for(int i = 0; i < numofthreads; i++){
        pthread_join(threads[i], NULL);
    }
    int total_primes = count;
    for(int i = limit; i < size; i++){
        if(prime[i]){
            total_primes++;
        }
    }
    // Calculate elapsed time
    clock_gettime(CLOCK_MONOTONIC, &finish);
    double elapsed;
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Number of primes %d\n", total_primes);
    printf("Time taken %.5lf seconds.\n", (float)elapsed);     
    
    // Free allocated memory
    free(prime);
    free(args);
    free(threads);
    return 0;
}

void* sieve(void* arg) {
    struct threadargs* data = (struct threadargs*) arg;

    for (int i = 0; i < data->size; i++) {
        int p = data->base_primes[i];

        // Find the first multiple of p in the thread's range
        int start_index = ((data->start + p - 1) / p) * p;
        if (start_index < p * p) start_index = p * p;

        for (int j = start_index; j <= data->end; j += p) {
            data->prime[j] = false;
        }
    }

    return NULL;
}