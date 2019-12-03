#include <stdio.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	
	int my_rank = -1;
	int process_count = -1;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int hostname_length = -1;
	MPI_Get_processor_name(hostname, &hostname_length);
	
	printf("Hello World from main thread of process %d of %d on %s\n", my_rank, process_count, hostname);
	
	#pragma omp parallel
	{
		#pragma omp critical (stdout)
		printf("\tHello World from thread %d of %d of process %d on %s\n", omp_get_thread_num(), omp_get_num_threads(), my_rank, hostname);
	} 
	
	MPI_Finalize();
	
	return 0;
}
