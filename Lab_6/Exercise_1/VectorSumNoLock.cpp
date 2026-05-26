# include <cstdlib>
# include <cstdio>
# include <cstring>
# include <cmath>
#include <thread>
#include <vector>
#include <sys/time.h>


double get_time ();
double f( double );

double get_time () {
	struct timeval t{};

	gettimeofday(&t, nullptr);
	return (t.tv_sec + t.tv_usec/1000000.0);
}


double f( double a )
{
	return (4.0 / (1.0 + a*a));
}


int main( int argc, char *argv[])
{
	double PI25DT = 3.141592653589793238462643;
	double *local_sum;
	int i;


	if (argc != 3) {
		printf ("Usage : %s <number_of_intervals> <number_of_threads>\n", argv[0]);
		return 1;
	}

	long int n = strtol(argv[1], nullptr, 10);
	long int threadCount = strtol(argv[2], nullptr, 10);
	double pi = 0.0;
	double h = 1.0 / static_cast<double>(n);
	double sum = 0.0;
	double interval = n / threadCount;
	double total_time = get_time();
	std::thread threads[threadCount];
	std::vector<double> SUM(threadCount);
	for (int i = 0 ; i < threadCount ; i++) {
		threads[i] = std::thread([=, &SUM]() { //All values are captured by value and &sum is captured by reference.
			double local_sum = 0;
			const int from = i * interval;
			const int to =  (i + 1) * interval;
			for (int j = from; j < to; j++) {
				double x = h * (j + 0.5);
				local_sum += f(x);
			}
			SUM[i] += local_sum;
		});
	}
	for (int i = 0 ; i < threadCount ; i++) {
		threads[i].join();
	}
	for (int i = 0 ; i < threadCount ; i++) {
		sum += SUM[i];
	}
	pi = h * sum;

	total_time = get_time() - total_time;
	printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
	printf("Total time (s): %f\n", total_time);


	return 0;
}

