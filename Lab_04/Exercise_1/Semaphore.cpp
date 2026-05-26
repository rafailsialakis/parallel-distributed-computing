#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <semaphore>

#define NUM_THREADS 10
#define CONSUMERS 1

std::mutex mtx;
std::binary_semaphore semaphore(0);
std::binary_semaphore mutex(0);

static int S = 0;

class Producer {
	public:
	Producer() {
		std::cout << "Producer starts" << std::endl;
	}

	void produce() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
			std::unique_lock<std::mutex> lock(mtx);
			S++;
			std::cout << "Producer produces item " << S << std::endl;
			semaphore.release();
		}
	}
};

class Consumer {
	int id;
	public:
	Consumer(const int id) {
		this->id = id;
		std::cout << "Consumer " << id << " starts" << std::endl;
	}

	void consume() {
		while (true) {
			semaphore.acquire();
			std::unique_lock<std::mutex> lock(mtx);
			std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate time to consume an item
			std::cout << "Consumer " << id << " consumes item " << S << std::endl;
			S--; // Decrement the shared resource
		}
	}
};

int main() {
	Producer producer;
	Consumer* consumer[CONSUMERS];
	std::thread consumer_thread[CONSUMERS];
	//Start the producer...
	std::thread producer_thread(&Producer::produce, &producer);
	//Start the consumers...
	for (int i = 0; i < CONSUMERS; i++) {
		consumer[i] = new Consumer(i);
		consumer_thread[i] = std::thread(&Consumer::consume, consumer[i]);
	}
	producer_thread.join();
	for (int i = 0; i < CONSUMERS; i++) {
		consumer_thread[i].join();
	}
	return 0;
}
