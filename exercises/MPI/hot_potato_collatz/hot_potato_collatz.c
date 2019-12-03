#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	MPI_Init( &argc, &argv );
  	
	int my_rank = -1; 
	int process_count = -1;
	long long potato = 0; 
	int initial_process = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);

	if(argc > 2 )
	{
		potato = strtoull(argv[1], NULL, 10);
		initial_process = atoi(argv[2]);       
		if((long long)potato < 0 || initial_process < 0)
			return printf("%s\n", "Must be positive"), 2; 
	}
	else 
		return printf("%s\n", "No potato given nor initial process"), 1;
		
	long long my_potato = potato;
	
	int prev = (my_rank - 1 + process_count) % process_count; 
	int next = (my_rank + 1) % process_count; 
	
	int total = process_count; 
	
	int alive = 1; 

	if(my_rank == initial_process)
	{
		MPI_Send(&my_potato, 1, MPI_LONG_LONG_INT, next, 0, MPI_COMM_WORLD); 
		MPI_Send(&total, 1, MPI_INT, next, 0, MPI_COMM_WORLD); 
	}
	
			
	while( my_potato > 1 && total > 1)
	{
		MPI_Recv(&my_potato, 1, MPI_LONG_LONG_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
		MPI_Recv(&total, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
		if(alive != 0)
		{
			if(my_potato % 2 == 0)
				my_potato /= 2;
			else 
				my_potato = 3*my_potato + 1; 
		}
		if( my_potato == 1)
		{
			--total;
			my_potato = potato; 
			alive = 0;
		}
		MPI_Send(&my_potato, 1, MPI_LONG_LONG_INT, next, 0, MPI_COMM_WORLD);
		MPI_Send(&total, 1, MPI_INT, next, 0, MPI_COMM_WORLD); 
		if(total == 1 && alive != 0)
			printf("Thread %d got the collatz victory royale.\n", my_rank);
	}
	
	MPI_Finalize(); 

	return 0;
}
