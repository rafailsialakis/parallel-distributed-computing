//
// Created by rafail on 3/15/25.
//
#include <iostream>
#include <mutex>
#include "Fork.h"

#ifndef PHILOSHOPER_H
#define PHILOSHOPER_H

inline std::mutex waiter;
inline std::mutex coutMutex;

class Philosopher {
private:
    int identity;
    Fork* left;
    Fork* right;
    int next;
public:
    Philosopher(int id, int n, Fork* l, Fork* r)
        : identity(id), next(n), left(l), right(r) {}

    /*
     The philosophers are in a queue. There is a waiter. The waiter gives forks to each philosopher, and he waits for them to finish
     dinner in order to acquire the forks for the next philosopher in the queue to use them. This implementation does not guarantee
     the most parallelism because only 2 forks are used each time.
     */
    [[noreturn]] void operator()() {
        while (true) {
            think();
            waiter.lock();
            getLeft();
            getRight();
            eat();
            releaseLeft();
            releaseRight();
            waiter.unlock();

        }
    }

    void think() {
        coutMutex.lock();
        std::cout << "Philosopher " <<  identity << " is thinking" << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Thinking for 0.5 second
        coutMutex.unlock();
    }

    void eat() {
        coutMutex.lock();
        std::cout << "Philosopher " <<  identity <<" is eating\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Thinking for 0.5 second
        coutMutex.unlock();
    }

    void getRight() {
        coutMutex.lock();
        std::cout << "Philosopher " <<  identity << " is trying to get right fork " << identity << "\n";
        right->get();
        coutMutex.unlock();
    }

    void getLeft() {
        coutMutex.lock();
        std::cout << "Philosopher " <<  identity << " is trying to get left fork " << next << "\n";
        left->get();
        coutMutex.unlock();
    }

    void releaseRight() {
        coutMutex.lock();
        std::cout << "Philosopher " <<  identity << " is releasing right fork " << identity << "\n";
        right->put();
        coutMutex.unlock();
    }

    void releaseLeft() {
        coutMutex.lock();
        std::cout << "Philosopher " <<  identity << " is releasing left fork " << next << "\n";
        left->put();
        coutMutex.unlock();
    }
};



#endif //PHILOSHOPER_H
