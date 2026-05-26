#include <iostream>
#include <thread>
#include <mutex>  // Include mutex for synchronization

std::mutex mtx;  // Mutex to synchronize access to the array
void run(int end, int* array);
void check_array(int end, int* array, int numThreads);

int main(){

    /*
    Variables are defined and being passed from symbol main()
    */
    int end = 1000;
    int* array = new int[end]();
    int numThreads = 4;

    std::thread threads[numThreads];
    for(int i = 0; i < numThreads; i++){
        threads[i] = std::thread(run, end, array);
    }
    for(int i = 0; i < numThreads; i++){
        threads[i].join();
    }
    check_array(end, array, numThreads);
    return 0;
}

void run(int end, int* array){
    for (int i = 0; i < end; i++) {
        mtx.lock(); //Same practice for the mutex not being outside of both looks so as to achieve more parallelism.
        for (int j = 0; j < i; j++)
            array[i]++;
        mtx.unlock();
    } 
}

void check_array(int end, int* array, int numThreads){
    int i, errors = 0;
    std::cout << "Checking..." << std::endl;

    for (i = 0; i < end; i++) {
        if (array[i] != numThreads*i) {
            errors++;
            printf("%d: %d should be %d\n", i, array[i], numThreads*i);
        }         
    }
    std::cout << errors << " errors." << std::endl;
}