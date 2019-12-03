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
	

	MPI_Allreduce(&my_number, &all_max, 1, MPI_LONG, MPI_MAX, MPI_COMM_WORLD);
	MPI_Allreduce(&my_number, &all_min, 1, MPI_LONG, MPI_MIN, MPI_COMM_WORLD);
	MPI_Allreduce(&my_number, &all_sum, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
	
	if(my_number == all_max)
		printf("Process's #%d lucky number (%ld) is the maximum\n",my_rank,my_number);
		
	if(my_number == all_min)
		printf("Process's #%d lucky number (%ld) is the minimum\n",my_rank,my_number);
	
	if(my_number > all_sum / process_count)
		printf("Process's #%d lucky number (%ld) is greater than current avarage (%.2lf)\n",my_rank,my_number,(double)all_sum/process_count);
	
	if(my_number < all_sum / process_count)
		printf("Process's #%d lucky number (%ld) is lower than current avarage (%.2lf)\n",my_rank,my_number,(double)all_sum/process_count);
	
	MPI_Finalize();
	
	
	return 0;
}
