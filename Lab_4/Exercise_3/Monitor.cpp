#include <condition_variable>
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>

#define CAPACITY 4
#define SCALE 10

class Monitor{
private:
	int capacity;
	int spaces;
	int waitscale;
	int inscale;
	std::mutex mutex;
	std::condition_variable cv;

public:
	Monitor(int c) {
		capacity = c;
		spaces = capacity;
		waitscale = 100;
		inscale = 5;
	}

	void arrive () {
		std::this_thread::sleep_for(std::chrono::milliseconds(waitscale * SCALE));
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock, [&](){return spaces > 0;});
		std::cout << std::this_thread::get_id() << " arrival" << std::endl;
		std::cout << std::this_thread::get_id() << " parking" << std::endl;
		spaces--;
		std::cout << "Free spaces: " << spaces << std::endl;
	}

	void depart () {
		std::cout << std::this_thread::get_id() << " departing" << std::endl;
		std::unique_lock<std::mutex> lock(mutex);
		spaces++;
		std::cout << "Free spaces: " << spaces << std::endl;
		cv.notify_all();
	}

	void park() {
		std::this_thread::sleep_for(std::chrono::milliseconds(inscale * SCALE));
	}
};


class Car {
private:
    Monitor* monitor;
public:
    Car(Monitor* a) : monitor(a) {}

	void run() {
		for (int i = 0; i < 100; i++) {
			monitor->arrive();
			monitor->park();
			monitor->depart();
		}
	}
};

int main() {
	int capacity = 4;
	Monitor monitor(capacity);
	int cars = 10;
	std::vector<std::thread> threads;
	threads.reserve(cars);
	for (int i = 0; i < cars; i++) {
		threads.emplace_back([&monitor]() { Car(&monitor).run();});
	}
	for (auto& t : threads) {
		t.join();
	}
}

