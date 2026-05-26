/*
 * Value            | 10000         | 100000        | 1000000       | 10000000      | 100000000
 * Recursion_Limit  | 5000          | 50000`        | 100000        | 1000000       | 25000000
 * Time             | 0.000260s     | 0.000376s     | 0.002306s     | 0.017461s     | 0.113709s
 * Block Time       | 0.000454s     | 0.000547s     | 0.002395s     | 0.019544s     | 0.110033s
 * CPU Intel(R) Core(TM) i5-1035G1 CPU @ 1.00GHz
 */

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <pthread.h>
# include <sys/time.h>

double get_time ();
double f( double );
void* RecursivePiCalc(void* args);

typedef struct{
    int     recursion_limit;
    int     left;
    int     right;
    double  sum;
    double  h;
}threadargs;

int main( int argc, char *argv[])
{
    double total_time;
    
    double PI25DT = 3.141592653589793238462643;
    double pi, h, x;
    double *local_sum;
    int i;

   
    if (argc != 3) {
		printf ("Usage : %s <number_of_intervals> <recursion_limit>\n", argv[0]);
		return 1;
    }
    
    long int n = strtol(argv[1], NULL, 10);
    long int rlimit = strtol(argv[2], NULL, 10);

    h  = 1.0 / (double) n;
    total_time = get_time(); 
    threadargs args = {rlimit, 1, n, 0.0, h};
    RecursivePiCalc(&args);
    pi = h * args.sum;

    total_time = get_time() - total_time; 
    printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
    printf("Total time (s): %f\n", total_time);

        
    return 0;
}

double get_time () {
	struct timeval t;
        gettimeofday(&t, NULL); 
        return (t.tv_sec + t.tv_usec/1000000.0);
}

double f( double a ){
    return (4.0 / (1.0 + a*a));
} 

void* RecursivePiCalc(void* args){
    threadargs* argv = (threadargs*) args;
    pthread_t rthread, lthread;
    int rlimit = argv->recursion_limit;
    int left = argv->left;
    int right = argv->right;
    int workload = right-left;
    int mid;
    double h = argv->h;
    double x;
    if(workload <= rlimit){
        for(int i = left; i <= right; i++){
            x = h * ((double)i - 0.5);
            argv->sum += f(x);
        }
    }
    else{
        mid = (left+right)/2;
        threadargs* largs = malloc(sizeof(threadargs));
        threadargs* rargs = malloc(sizeof(threadargs));

        *largs = (threadargs){rlimit, left, mid, 0.0, h};
        *rargs = (threadargs){rlimit, mid + 1, right, 0.0, h};
        pthread_create(&rthread, NULL, RecursivePiCalc, rargs);
        pthread_create(&lthread, NULL, RecursivePiCalc, largs);
        pthread_join(lthread, NULL);
        pthread_join(rthread, NULL);
        argv->sum = largs->sum + rargs->sum;
        free(largs);
        free(rargs);
    }
    return NULL;
}

            