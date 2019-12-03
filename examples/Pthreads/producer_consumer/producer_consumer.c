#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	void *(*start_routine)(void*), void *arg);
*/

// thread shared data
typedef struct
{
	size_t thread_count;
	size_t data_size;
	double* data;
	pthread_mutex_t mutex;
	size_t rounds;
	unsigned producer_max_delay; //microseconds
	unsigned consumer_max_delay; //microseconds
  sem_t producer_semaphore;
	sem_t consumer_semaphore;
} shared_data_t;

void *produce(void* data)
{
	shared_data_t* shared_data = (shared_data_t*)data;

	for(size_t round = 1; round <= shared_data->rounds; ++round)
	{
		for(size_t index = 0; index < shared_data->data_size; ++index)
		{
			sem_wait(&shared_data->producer_semaphore);

			// Producer requires time to produce a value
			usleep( rand() % (shared_data->producer_max_delay + 1)*1000 );
			double product = round + (index+1) / 100.0;

			shared_data->data[index] = product;
			pthread_mutex_lock(&shared_data->mutex);
			printf("Produced %.2f\n",product);
			pthread_mutex_unlock(&shared_data->mutex);

			sem_post(&shared_data->consumer_semaphore);
		}
	}
	return NULL;
}

void *consume(void* data)
{
	shared_data_t* shared_data = (shared_data_t*)data;

	for(size_t round = 1; round <= shared_data->rounds; ++round)
	{
		for(size_t index = 0; index < shared_data->data_size; ++index)
		{
			sem_wait(&shared_data->consumer_semaphore);

			// Producer requires time to produce a value
			usleep( rand() % (shared_data->consumer_max_delay + 1)*1000 );
			double product = shared_data->data[index];

			pthread_mutex_lock(&shared_data->mutex);
			printf("\t\t\tConsumed %.2f\n",product);
			pthread_mutex_unlock(&shared_data->mutex);

			sem_post(&shared_data->producer_semaphore);
		}
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	shared_data_t shared_data;
	shared_data.thread_count = 2;
	pthread_mutex_init(&shared_data.mutex, NULL);

	if( argc == 5 )
	{
		if ( sscanf(argv[1], "%zu", &shared_data.data_size) != 1 || shared_data.data_size == 0 )
			return (void) fprintf(stderr, "hello_w: error: invalid thread count: %s\n", argv[1]), 1;
		if ( sscanf(argv[2], "%zu", &shared_data.rounds) != 1 || shared_data.rounds == 0 )
			return (void) fprintf(stderr, "producer consumer: error: invalid rounds: %s\n", argv[2]), 1;
		if ( sscanf(argv[3], "%u", &shared_data.producer_max_delay) != 1 || shared_data.producer_max_delay == 0 )
			return (void) fprintf(stderr, "producer consumer: error: invalid producer max delay: %s\n", argv[3]), 1;
		if ( sscanf(argv[4], "%u", &shared_data.consumer_max_delay) != 1 || shared_data.consumer_max_delay == 0 )
		  return (void) fprintf(stderr, "producer consumer: error: invalid consumer max delay: %s\n", argv[4]), 1;
	}
	else
			return (void)fprintf(stderr, "usage: producer_consumer data_size rounds producer_delay consumer_delay \n"), 1;

	shared_data.data = (double*) calloc(shared_data.data_size, sizeof(double));
	if( shared_data.data == NULL)
			return 6;

 	sem_init(&shared_data.producer_semaphore, 0, shared_data.data_size);
	sem_init(&shared_data.consumer_semaphore, 0, 0);

	pthread_t* threads = (pthread_t*) malloc(shared_data.thread_count * sizeof(pthread_t));
	if ( threads == NULL )
		return (void) fprintf(stderr, "hello_w: error: invalid thread count: %s\n", argv[1]), 1;

	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);

	// Create producer
	pthread_create(&threads[0], NULL, produce, &shared_data);
	// Create consumer
	pthread_create(&threads[1], NULL, consume, &shared_data);

	for( size_t index = 0;index < shared_data.thread_count; ++index )
		pthread_join(threads[index], NULL);


	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);

	double seconds = finish_time.tv_sec - start_time.tv_sec + (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9 ;

	printf("\nThread creation and join time: %.15lfs\n", seconds );

	pthread_mutex_destroy(&shared_data.mutex);
	sem_destroy(&shared_data.consumer_semaphore);
	sem_destroy(&shared_data.producer_semaphore);
	free(threads);
	free(shared_data.data);

	return 0;
}
