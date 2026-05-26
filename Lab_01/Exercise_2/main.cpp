#include <iostream>
#include <thread>

/*\
Program in modern C++ to calculate the multiplication of i+1 for 0<=i<=9
Non-deterministic output. We observe that when we put all threads to sleep except
one, in this example thread[0], we can isolate its output so it is printed correctly.
However we observe non-deterministic behaviour after all the other threads are awake.
*/

void multiplierThread(int num){
    if (num != 0){
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
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
