#include <iostream>
#include <thread>

/*
Use of shared struct with the data to be passed
*/
struct Data{
    int end = 1000;
    int* array = new int[end]();
    int numThreads = 4;
};

void run(Data* data);
void check_array(Data* data);

int main(){

    struct Data data;
    std::thread threads[data.numThreads];
    for(int i = 0; i < data.numThreads; i++){
        threads[i] = std::thread(run, &data);
    }
    for(int i = 0; i < data.numThreads; i++){
        threads[i].join();
    }
    check_array(&data);
    return 0;
}

void run(Data* data){
    for (int i = 0; i < data->end; i++) {
        for (int j = 0; j < i; j++)
            data->array[i]++;		
    } 
}

void check_array(Data* data){
    int i, errors = 0;
    std::cout << "Checking..." << std::endl;

    for (i = 0; i < data->end; i++) {
        if (data->array[i] != data->numThreads*i) {
            errors++;
            printf("%d: %d should be %d\n", i, data->array[i], data->numThreads*i);
        }         
    }
    std::cout << errors << " errors." << std::endl;
}