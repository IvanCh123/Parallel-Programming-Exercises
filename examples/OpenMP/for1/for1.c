#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int thread_count = omp_get_max_threads();
	if( argc >= 2 )
			thread_count = atoi(argv[1]);

	int iterations = thread_count;
	if( argc >= 3)
			iterations = atoi( argv[2]);

	#pragma omp parallel for num_threads(thread_count) \
					default(none) shared(iterations)
	for(int iteration = 0; iteration < iterations; ++iteration)
	{
		#pragma omp critical (print)
		printf("%d/%d: iteration %d/%d\n",omp_get_thread_num(), omp_get_num_threads(), iteration, iterations );
	}
	return 0;
}
