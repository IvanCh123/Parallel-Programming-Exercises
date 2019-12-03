#include <stdio.h>
#include <string.h>
#include <mpi.h>

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
	
	char message[2048];
	
	if( my_rank != 0)
	{
		sprintf(message, "Hello World from main thread of process %d of %d on %s", my_rank, process_count, hostname);
		MPI_Send(message, strlen(message)+1, MPI_CHAR, 0 /*destino*/, 0 /*tag*/, MPI_COMM_WORLD);
	}else
	{
		for(int source = 1; source < process_count; source++)
		{
			MPI_Recv(message, /*buffer size*/2048, MPI_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("[%d] %s\n", source, message);
		}
	}
	MPI_Finalize();
	
	return 0;
}
