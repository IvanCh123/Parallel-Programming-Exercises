#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

double function(double x){
	double result=0;
	result=x*x;
	result+=x;
	result+=1;
	
	return result;
}

void trapezoidal_area (double a, double b, double* area)
{

	double temp=(b-a);
	temp*=(function(a)+function(b))/2;
	
	#pragma omp critical
	*area += temp;
}

int main(int argc, char* argv[])
{
	
	double 	a=0;
	double	b=0;
	int   	n=0;
	int 	worker_count=sysconf(_SC_NPROCESSORS_ONLN);
	double	area=0;
	double 	step=0;
	sscanf(argv[1], "%lf", &a);
	sscanf(argv[2], "%lf", &b);
	sscanf(argv[3], "%d", &n);
	if(argc>4)
		sscanf(argv[4], "%d", &worker_count);
	step = (b-a)/(n*1.0);
	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	
	#pragma omp parallel for num_threads(worker_count) default(none) shared(area,step,a,b,n)
	for(int index=0; index<n;++index)
	{
		trapezoidal_area((a+step*index),(a+step*(index+1)), &area);
	}	
	
	
	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);
	
	double seconds= finish_time.tv_sec - start_time.tv_sec + 
	(finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
	
	printf("The area between %lf and %lf is %lf. Execution time was %lf\n", a,b,area,seconds);
	
}
