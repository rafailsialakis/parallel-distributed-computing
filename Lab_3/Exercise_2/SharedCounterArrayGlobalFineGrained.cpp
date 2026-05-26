#include <iostream>
#include <thread>
#include <mutex>

/*
SharedCounterArrayGlobal.java translated to C++
*/

void run();
void check_array();

int end = 1000;
int* array = new int[end]();
std::mutex mtx[1000];  //Array of mutex objects, so as each element of the array has its own lock.

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
    return 0;
}

void run(){
    for (int i = 0; i < end; i++) {
        mtx[i].lock();
        for (int j = 0; j < i; j++)
            array[i]++;
        mtx[i].unlock();
    }
}


void check_array(){
    int errors = 0;
    std::cout << "Checking..." << std::endl;
    for (int i = 0; i < end; i++) {
        if (array[i] != numThreads*i) {
            errors++;
            printf("%d: %d should be %d\n", i, array[i], numThreads*i);
        }
    }
    std::cout << errors << " errors." << std::endl;
}