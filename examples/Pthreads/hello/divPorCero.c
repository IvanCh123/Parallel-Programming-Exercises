#include <pthread.h>
#include <stdio.h>

/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	void *(*start_routine)(void*), void *arg);
*/	
	
void *run(void* data)
{
	size_t thread_id = (size_t)data/0;
	printf("Hello from thread %zu\n", thread_id);
	return NULL;
}

int main(void)
{
	pthread_t thread; // crea el reg en la pila del main
	pthread_create(&thread, NULL, run, (void*)13);
	
	printf("Hello from main thread\n");
	pthread_join(thread, NULL);
	return 0;
}
