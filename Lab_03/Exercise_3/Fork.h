//
// Created by rafail on 3/15/25.
//

#ifndef FORK_H
#define FORK_H

#include <mutex>

class Fork {

private:
    int identity;
    std::mutex lock;

public:
    explicit Fork(const int id):
    identity(id){}

    void get() {
        lock.lock();
    }

    void put() {
        lock.unlock();
    }
};

#endif //FORK_H
