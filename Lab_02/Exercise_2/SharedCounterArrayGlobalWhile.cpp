#include <iostream>
#include <thread>
#include <mutex>
/*
SharedCounterArrayGlobalWhile.java translated to C++
*/

void check_array();
void run();

std::mutex mutex;
int end = 10000;
int counter = 0;
int* array = new int[end](); //Initialize array to 0
int numThreads = 4;

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
    while(true){
        mutex.lock();
        if (counter >= end) {
            mutex.unlock(); //If we don't unlock the mutex after counter >= end, deadlock...
            break;
        }
        array[counter]++;
        counter++;
        mutex.unlock();
    }
}