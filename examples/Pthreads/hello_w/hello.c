#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	void *(*start_routine)(void*), void *arg);
*/

void *run(void* data)
{
	size_t thread_id = (size_t)data;
	printf("Hello from thread %zu\n", thread_id);
	return NULL;
}

int main(int argc, char* argv[])
{


#if defined(PRINT_ARGS)
	for(int index = 0; index < argc; ++index)
		printf("%d[%s]\n", index, argv[index])
#endif

	size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
	if( argc >= 2 )
	{
		//thread_count = strtoull (argv[1], NULL, 10); //Casteamos el primer argumento a entero en base 10
		if ( sscanf(argv[1], "%zu", &thread_count) != 1 || thread_count == 0 )
			return (void) fprintf(stderr, "hello_w: error: invalid thread count: %s\n", argv[1]), 1;
	}

	pthread_t* threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
	if ( threads == NULL )
		return (void) fprintf(stderr, "hello_w: error: could not allocate memory for: %zu threads\n", thread_count), 2;

	//int clock_gettime(clockid_t clk_id, struct timespec *tp);
	struct timespec start_time;
 	clock_gettime(CLOCK_MONOTONIC, &start_time);

	for( size_t index = 0; index < thread_count; ++index )
		pthread_create(&threads[index], NULL, run, (void*)index);

	printf("Hello from main thread\n");

	for( size_t index = 0;index < thread_count; ++index )
		pthread_join(threads[index], NULL);

#if 0
	struct timespec {
		time_t   tv_sec;        /* seconds */
		long     tv_nsec;       /* nanoseconds */
	};
#endif

	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);

	double seconds = finish_time.tv_sec - start_time.tv_sec + (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9 ;

	printf("Thread creation and join time: %.15lfs\n", seconds );
	free(threads);

	return 0;
}
