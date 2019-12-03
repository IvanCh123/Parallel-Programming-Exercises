#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int thread_count = omp_get_max_threads();
	if( argc >= 2 )
			thread_count = atoi(argv[1]);
	#pragma omp parallel num_threads(thread_count)
	{
		#pragma omp critical (print)
		printf("Hello from thread #%d of %d\n",omp_get_thread_num(), omp_get_num_threads() );
	}
	return 0;
}
