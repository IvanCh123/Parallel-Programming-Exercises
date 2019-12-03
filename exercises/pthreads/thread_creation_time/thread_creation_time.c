#include <pthread.h>
#include <stdio.h>
#include <time.h>


void* run(void *unused)
{
	(void)unused;
	return NULL;
}

int main(int argc, char* argv[]){
	size_t iterations;
	if(argc>=2){
	    sscanf(argv[1], "%zu", &iterations);
	}
	else{
		iterations = 1;
    }
    struct timespec start_time;
    struct timespec finish_time;
    double time = 10;
    double tmp;
    for(size_t index = 0; index < iterations; ++index){
		clock_gettime(CLOCK_MONOTONIC, &start_time);
		pthread_t thread;
		pthread_create(&thread, NULL, run, NULL);
		pthread_join(thread, NULL);
		clock_gettime(CLOCK_MONOTONIC, &finish_time);
		tmp = finish_time.tv_sec - start_time.tv_sec
	    + (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
	    if (tmp < time)
	        time = tmp;
	}
	printf("Minimum thread creation and destruction time was %lf among %zu trials\n", time, iterations);
}
