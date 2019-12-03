#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define MIN(a,b) ( (a) < (b) ? (a) : (b))

size_t calculate_start( size_t data_count, size_t worker_count, size_t worker_id )
{
	return worker_id * (data_count / worker_count) + MIN( worker_id, data_count % worker_count);
}

size_t calculate_finish( size_t data_count, size_t worker_count, size_t worker_id )
{
	return calculate_start(data_count, worker_count, worker_id+1);
}

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
	
	size_t start = 0;
	size_t finish = 0;
	
	if( argc == 3 )
	{	
		start = strtoull( argv[1], NULL, 10);
		finish = strtoull( argv[2], NULL, 10);
	}else
	{
		if( my_rank == 0 )
		{
			if( scanf("%zu %zu", &start, &finish) != 2 )
				return (void)fprintf(stderr, "usage: hybrid_distr_arg min max\n"), EXIT_FAILURE; 
		}
		MPI_Bcast(&start, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);	
		MPI_Bcast(&finish, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
	}
	

	
	size_t my_start = start + calculate_start( finish-start, (size_t)process_count, (size_t)my_rank);
	size_t my_finish = start + calculate_finish( finish-start, (size_t)process_count, (size_t)my_rank);
	
	size_t size = my_finish - my_start;
	
	printf("%s:%d range[%zu, %zu[ size %zu\n", hostname ,my_rank, my_start, my_finish, size);
	
	#pragma omp parallel default(none) shared(my_rank, process_count, hostname, my_start, my_finish)
	{
		size_t thread_start = 0;
		size_t thread_finish = 0;
		
		#pragma omp for
		for( size_t current = my_start; current < my_finish; ++current )
		{
			if( thread_start == 0 )
				thread_start = current;
				
			thread_finish = current;
		} 
		
		++thread_finish;
		#pragma omp critical (stdout)
		printf("\t%s:%d.%d: range[%zu, %zu[ size %zu\n", hostname, my_rank, omp_get_thread_num(), thread_start, thread_finish, thread_finish - thread_start + 1 );
		
		
	}
	MPI_Finalize();	
	return 0;	
}
