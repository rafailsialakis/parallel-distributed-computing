#include <iostream>
#include <thread>
#include <mutex>

/*
Lk is created and m is locked (m is a shared resource). After executing the loop or the
critical section the mutex is unlocked and the program continues...
*/
#define synchronized(m) \
for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())


/*
SharedCounterArrayGlobal.java translated to C++
*/

void run();
void check_array();

std::recursive_mutex mutex;
int end = 1000;
int* array = new int[end]();
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
        synchronized(mutex){ //Instead of using the lock of an object we use a recursive_mutex as we define synchronized keyword.
            for (int j = 0; j < i; j++)
                //Synchronized could also be inside the second for loop to secure more parallel behaviour without errors.
                array[i]++;
        }
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