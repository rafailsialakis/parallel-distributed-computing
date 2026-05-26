#include <iostream>
#include <thread>
#include <mutex>
#include "Fork.h"
#include "Philosopher.h"

const int numPhilosophers = 5;  // Number of philosophers// Sleep time for thinking/eating

int main() {
    Fork* forks[numPhilosophers];
    Philosopher* philosophers[numPhilosophers];

    for (int i = 0; i < numPhilosophers; ++i) {
        forks[i] = new Fork(i);  // Dynamically allocate each fork
    }

    for (int i = 0; i < numPhilosophers; i++){
        philosophers[i] = new Philosopher(i, (i+1)%numPhilosophers, forks[i], forks[(i+1)%numPhilosophers]);
    }

    // Create and start threads for each philosopher
    std::thread threads[numPhilosophers];
    for (int i = 0; i < numPhilosophers; ++i) {
        threads[i] = std::thread(&Philosopher::operator(), philosophers[i]);
    }

    // Join all threads (wait for all philosophers to finish)
    for (int i = 0; i < numPhilosophers; ++i) {
        threads[i].join();
    }

    for (int i = 0; i < numPhilosophers; ++i) {
        delete forks[i];
        delete philosophers[i];
    }

    return 0;
}
