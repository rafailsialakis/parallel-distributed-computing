#include <iostream>
#include <thread>
#include <mutex>
/*
SharedCounterArrayGlobalWhile.java translated to C++
*/
#define synchronized(m) \
for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())

void check_array();
void run();

int end = 10000;
int counter = 0;
int* array = new int[end]();
int numThreads = 4;
std::recursive_mutex mutex[10000]; //Use of an array of recursive_mutexes in order for each item of the matrix to have its own unique lock

int main(){
    std::thread threads[numThreads];
    for(int i = 0; i < numThreads; i++){
        threads[i] = std::thread(run);
    }
    for(int i = 0; i < numThreads; i++){
        threads[i].join();
    }
    check_array();

    delete[] array;
    return 0;
}

void check_array(){
    int i, errors = 0;
    std::cout << "Checking..." << std::endl;
    for(i = 0; i < end; i++){
        if(array[i] != 1){
            errors++;
            std::cout << i << ": " << array[i] << " should be 1" << std::endl;
        }
    }
    std::cout << errors << " errors." << std::endl;
}

void run(){
    while(true) {
        int i = 0;
        synchronized(mutex[i]) { //synchronized takes as a parameter a mutex of the mutex array.
            if (counter >= end) {
                break;
            }
            array[counter]++;
            counter++;
            i++;
        }
        if (counter >= end) {
            break;
        }
    }
}


