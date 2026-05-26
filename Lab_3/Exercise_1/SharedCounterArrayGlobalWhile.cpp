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


std::recursive_mutex mutex;
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
    while(true) {
        synchronized(mutex) {
            if (counter >= end) {
                break;
            }
            array[counter]++;
            counter++;
        }
        //Reuse of if statement in order to not be an infinite loop (deadlock) or a Segmentation Fault.
        if (counter >= end) {
            break;
        }
    }
}

// void run(){
//     while(true) {
//         synchronized(mutex) {
//             if (counter >= end) {
//                 break;
//             }
//             array[counter]++;
//             counter++;
//         }
//     }
// } This code ends up to a deadlock...

// void run(){
//     while(true) {
//         if (counter >= end) {
//             break;
//         }
//         synchronized(mutex){
//             array[counter]++;
//             counter++;
//         }
//     }
// } This code works fine but returns an munmap_chunk(): invalid pointer because many threads try to access the shared resource counter at the same time

