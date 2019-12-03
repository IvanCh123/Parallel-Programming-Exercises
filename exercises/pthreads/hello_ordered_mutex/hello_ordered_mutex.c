#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

    
typedef struct{
	size_t thread_count;
}shared_data_t;  
    
typedef struct {
	size_t thread_id;
	pthread_mutex_t mutex;
	shared_data_t* shared_data;
} private_data_t;
    
void* run(void* data)
{
	private_data_t* private_data = (private_data_t*)data;
	shared_data_t* shared_data = private_data->shared_data;
	pthread_mutex_lock(&private_data->mutex);
	printf("Hello from thread %zu of %zu\n", private_data->thread_id, shared_data->thread_count);
	pthread_mutex_unlock(&private_data->mutex);
	if(private_data->thread_id < shared_data->thread_count-1){
		pthread_mutex_unlock(&(private_data+1)->mutex);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	
#if defined(PRINT_ARGS)
	for(int index = 0; index < argc; ++index)
	    printf("%d[%s]\n", index, argv[index]);
#endif
	size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
	if( argc >= 2 ){
	    if(sscanf(argv[1], "%zu", &thread_count) != 1 || thread_count == 0){
		    return (void) fprintf(stderr, "hello_w: error: invalid threat count: %s\n", argv[1]), 1;
	    }
    }
	pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
	if( threads == NULL )
	    return (void) fprintf(stderr, "hello_w: error: could not allocate memory for: %zu threads\n", thread_count), 1;
	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	shared_data_t shared_data;
	shared_data.thread_count = thread_count;
	private_data_t* private_data = (private_data_t*) malloc(thread_count * sizeof(private_data_t));
	
	for(size_t mutex_index = 0; mutex_index < thread_count; ++mutex_index){
		pthread_mutex_init(&private_data[mutex_index].mutex,NULL);
	}
	
	for(size_t mutex_lock = 1; mutex_lock < thread_count; ++mutex_lock){
		
		pthread_mutex_lock(&private_data[mutex_lock].mutex);
	}
	
	for(size_t threadIndex = 0; threadIndex < thread_count; ++threadIndex){
		private_data[threadIndex].thread_id = threadIndex;
		private_data[threadIndex].shared_data = &shared_data;
	    pthread_create(&threads[threadIndex], NULL, run, &private_data[threadIndex]); 
    }
    
    
    for(size_t threadIndex = 0; threadIndex < thread_count; ++threadIndex){
	    pthread_join(threads[threadIndex], NULL); 
    }
    
    printf("Hello from main thread\n");
#if 0
    struct timespec {
		time_t tv_sec; /*seconds*/
		long  tv_nsec; /* nanoseconds */
};
#endif
    struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);
	double seconds = finish_time.tv_sec - start_time.tv_sec
	    + (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
	printf("Thread creation and join time: %.9lfs\n", seconds);
    free(private_data); 
    free (threads);
	return 0;
}
