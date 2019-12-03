#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct{
	double value;
	double area;
	double step;
	pthread_mutex_t mutex;
}shared_data_t; 

typedef struct{
	double a;
	double b;
	shared_data_t* shared_data;
}private_data_t; 


double parable(double x){
	double y=0;
	y = x * x;
	y += x;
	y += 1;
	
	return y;
}

void* trapezoidal_area (void* data){
	
	private_data_t* private_data=(private_data_t*) data;
	shared_data_t* shared_data = private_data->shared_data;
	
	double thread_area=0;
	double a = private_data->a;
	double b = private_data->b;
	double step = shared_data->step;
	thread_area += parable(a);
	thread_area += parable(b);
	a += step;
	while(a < b){
		thread_area += 2 * parable(a);
		a += step;
	}
	thread_area *= shared_data->value;
	
	pthread_mutex_lock(&shared_data->mutex);
	shared_data->area += thread_area;
	pthread_mutex_unlock(&shared_data->mutex);
	return NULL;	
}

int min (int number_a, int number_b){
	if(number_a> number_b)
		return number_b;
	else 
		return number_a;
}

int calculateStart(int n, int thread_count, int thread_id){
	const int equitative = thread_id * (n / thread_count);
	const int overload = min(thread_id, (n % thread_count));
	return equitative + overload;
}

int calculateFinish(int n, int thread_count, int thread_id){
	return calculateStart(n, thread_count, thread_id + 1);
}

int main(int argc, char* argv[]){
	
	double step=0;
	double a=0;
	double b=0;
	int n=0;
	int thread_count=sysconf(_SC_NPROCESSORS_ONLN);
	double area=0;
	
	if( argc >= 2 ){
		
	    if(sscanf(argv[1], "%lf", &a) != 1 ){
		    return (void) fprintf(stderr, "Invalid 'a' value %s\n", argv[1]), 1;
	    }
	    
	    if(sscanf(argv[2], "%lf", &b) != 1){
		    return (void) fprintf(stderr, "Invalid 'b' value %s\n", argv[2]), 1;
	    }
	    
	    if(sscanf(argv[3], "%d", &n) != 1){
		    return (void) fprintf(stderr, "Invalid 'n' value %s\n", argv[3]), 1;
	    }
	    
	    if(sscanf(argv[4], "%d", &thread_count) != 1){
		    return (void) fprintf(stderr, "Invalid thread count value %s\n", argv[4]), 1;
	    }
	    
    }

	
	pthread_t* threads= (pthread_t*) malloc(thread_count * sizeof(pthread_t));
	private_data_t* private_data = (private_data_t*)malloc(thread_count * sizeof(private_data_t));
	shared_data_t shared_data; 
	pthread_mutex_init(&shared_data.mutex,NULL);
	shared_data.area=area;
	
	step=(b-a)/n;
	shared_data.step=step;
	shared_data.value=step/2;

	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	
	for(int thread_index=0; thread_index<thread_count; ++thread_index){
		
		private_data[thread_index].a= a + step*calculateStart(n,thread_count,thread_index);
		private_data[thread_index].b= a + step*(calculateFinish(n,thread_count,thread_index));
		private_data[thread_index].shared_data=&shared_data;
		pthread_create(&threads[thread_index], NULL, trapezoidal_area, private_data + thread_index);
	}
	
	for(int thread_index=0; thread_index<thread_count; ++thread_index){
		pthread_join(threads[thread_index],NULL);
	}
	
	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);
	
	double seconds= finish_time.tv_sec - start_time.tv_sec + 
	(finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
	
	printf("The area between %lf and %lf is %lf. Execution time was %lf\n", a,b,shared_data.area,seconds);
	
	free(threads);
	free(private_data); 
}
