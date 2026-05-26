#include <iostream>
#include <thread>

void check_array(int* array, int end);
void run(int* array, int *counter, int end);

int main(){
    /*
    Variables are defined and being passed from symbol main()
    */
    int end = 10000;
    int counter = 0;  
    int* array = new int[end](); //Memory allocation with initialization to 0
    int numThreads = 4;

    std::thread threads[numThreads];
    for(int i = 0; i < numThreads; i++){
        threads[i] = std::thread(run, array, &counter, end); //Pass the counter by reference to the threads in order to be edited
    }
    for(int i = 0; i < numThreads; i++){
        threads[i].join();
    }
    check_array(array, end);
    delete[] array; 
    return 0;
}

void check_array(int* array, int end){
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

void run(int* array, int *counter, int end){
    while(true){
        if (*counter >= end) 
            break;
        array[*counter]++;
        (*counter)++;
    }
}