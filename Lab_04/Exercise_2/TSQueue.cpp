#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#define NUM_THREADS 10
#define CONSUMERS 1

//https://www.geeksforgeeks.org/implement-thread-safe-queue-in-c/
template <typename T>
class TSQueue {
private:
	// Underlying queue
	std::queue<T> m_queue;

	// mutex for thread synchronization
	std::mutex m_mutex;

	// Condition variable for signaling
	std::condition_variable m_cond;

public:
	// Pushes an element to the queue
	void push(T item)
	{

		// Acquire lock
		std::unique_lock<std::mutex> lock(m_mutex);

		// Add item
		m_queue.push(item);

		// Notify one thread that
		// is waiting
		m_cond.notify_one();
	}

	// Pops an element off the queue
	T pop()
	{

		// acquire lock
		std::unique_lock<std::mutex> lock(m_mutex);

		// wait until queue is not empty
		m_cond.wait(lock,
					[this]() { return !m_queue.empty(); });

		// retrieve item
		T item = m_queue.front();
		m_queue.pop();

		// return item
		return item;
	}
};

class Message {
private:
	std::string msg;

public:
	Message(std::string str){
		msg=str;
	}

	std::string getMsg() {
		return msg;
	}
};

static int S = 0;
static TSQueue<Message> queue;


class Producer {
	public:
	Producer() {
		std::cout << "Producer starts" << std::endl;
	}

	void produce() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
			Message msg("Item " + std::to_string(++S));
			queue.push(msg);
			std::cout << "Producer produces item " << msg.getMsg() << std::endl;
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
			Message msg = queue.pop();
			std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate time to consume an item
			std::cout << "Consumer " << id << " consumes item " << msg.getMsg() << std::endl;
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
