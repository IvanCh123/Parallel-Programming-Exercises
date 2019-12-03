#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	void *(*start_routine)(void*), void *arg);
*/

typedef struct
{
	size_t thread_id;
	size_t thread_count;
	size_t* next_print_index;
} thread_data_t;

void *run(void* data)
{
	//(thread_data_t*)data;
	thread_data_t* thread_data = (thread_data_t*)data;

	while( *thread_data->next_print_index < thread_data->thread_id){/*mamador*/}
	printf("Hello from thread %zu of %zu\n", (*thread_data).thread_id, thread_data->thread_count);
	++*thread_data->next_print_index;

	return NULL;
}

int main(int argc, char* argv[])
{
	size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
	if( argc >= 2 )
	{
		if ( sscanf(argv[1], "%zu", &thread_count) != 1 || thread_count == 0 )
			return (void) fprintf(stderr, "hello_w: error: invalid thread count: %s\n", argv[1]), 1;
	}

	pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
	if ( threads == NULL )
		return (void) fprintf(stderr, "hello_w: error: could not allocate memory for: %zu threads\n", thread_count), 2;

	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);
  // void* calloc (size_t num, size_t size); /
	thread_data_t* thread_data = (thread_data_t*) calloc(thread_count, sizeof(thread_data_t)); // {index, thread_count};

	size_t next_print_index = 0;
	for( size_t index = 0; index < thread_count; ++index )
	{
		thread_data[index].thread_id = index;
		thread_data[index].thread_count = thread_count;
		thread_data[index].next_print_index = &next_print_index;
		pthread_create(&threads[index], NULL, run, &thread_data[index]);
	}

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

	printf("\nThread creation and join time: %.15lfs\n", seconds );

	free(thread_data);
	free(threads);

	return 0;
}
