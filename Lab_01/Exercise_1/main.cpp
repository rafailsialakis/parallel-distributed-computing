#include <iostream>
#include <thread>

/*\
Program in modern C++ to calculate the multiplication of i+1 for 0<=i<=9
Non-deterministic output. 
*/

void multiplierThread(int num){
    for(int i = 0; i < 20; i++){
        std::cout<<(num+1)<<" * "<<i<<" = "<<(num+1)*i<<std::endl;
    }
}


int main(){
    std::thread threads[10];

    for(int i = 0; i < 10; i++){
        threads[i] = std::thread(multiplierThread, i);
    }

    for(int i = 0; i < 10; i++){
        threads[i].join();
    }

    return 0;
}
