#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define MESSAGE_CAPACITY 2048

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
	
	srand( my_rank * time(NULL) );
	long my_number = rand() %100;
	
	//char message[MESSAGE_CAPACITY];
	
	long all_max = 0;
	long all_min = 0;
	long all_sum = 0;
	
	
	MPI_Reduce(&my_number, &all_max, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&my_number, &all_min, 1, MPI_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce(&my_number, &all_sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if(my_rank == 0)
	{
		
		printf("\nMax number %ld\n", all_max);
		printf("\nMin number %ld\n", all_min);
		printf("\nAll sum: %ld\n", all_sum);
	}
	

	
	MPI_Finalize();
	
	
	return 0;
}
