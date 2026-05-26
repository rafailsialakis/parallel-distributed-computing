#include <iostream>
#include <thread>
#include <mutex>
/*
Use of shared struct with the data to be passed
*/
struct Data
{
    int end = 10000;
    int counter = 0;  
    int* array = new int[end](); //Memory allocation with initialization to 0
    int numThreads = 4; 
};

void check_array(Data* data);
void run(Data* data);
std::mutex mutex;
int main(){
    struct Data data;
    std::thread threads[data.numThreads];
    for(int i = 0; i < data.numThreads; i++){
        threads[i] = std::thread(run, &data); //Pass the struct by reference to the threads in order to be edited
    }
    for(int i = 0; i < data.numThreads; i++){
        threads[i].join();
    }
    check_array(&data);
    return 0;
}

void check_array(Data* data){
    int i, errors = 0;
    std::cout << "Checking..." << std::endl;
    for(i = 0; i < data->end; i++){
        if(data->array[i] != 1){
            errors++;
            std::cout << i << ": " << data->array[i] << " should be 1" << std::endl;
        }
    }
    std::cout << errors << " errors." << std::endl;
}

void run(Data* data){
    while(true){
        mutex.lock();
        if(data->counter >= data->end){
            mutex.unlock();
            break;
        }
        data->array[data->counter]++;
        data->counter++;
        mutex.unlock();
    }
}