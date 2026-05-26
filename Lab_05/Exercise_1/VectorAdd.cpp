#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

void matrix_addition(double* a, double* b, double* c, int from, int to, int size) {
	for(int i = from; i < to; i++) {
		a[i] = b[i] + c[i];
	}
}

int main(int argc, char *argv[]) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	int size = argc > 1 ? atoi(argv[1]) : 1000;
	int block, from, to;

	double* a = new double[size];
	double* b = new double[size];
	double* c = new double[size];


	for(int i = 0; i < size; i++){
			a[i] = 0.0;
			b[i] = 1.0;
			c[i] = 0.5;
	}
	unsigned int num_cpus = std::thread::hardware_concurrency();
	block = size / num_cpus;
	std::vector<std::thread> threadPool;

	start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < num_cpus; i++) {
		from = i * block;
		to = std::min(from + block, size); // Prevent out-of-bounds access
		threadPool.emplace_back(matrix_addition,a,b,c,from,to,block);

	}
	for(int i = 0; i < num_cpus; i++) {
		threadPool[i].join();
	}

	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "Time taken: " << elapsed_seconds.count() * 1000 << " ms" << std::endl;

	delete[] a;
	delete[] b;
	delete[] c;

	return 0;
}
