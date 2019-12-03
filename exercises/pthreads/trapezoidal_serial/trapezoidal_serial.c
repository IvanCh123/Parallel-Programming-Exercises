#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

double parable(double x){
	double y = 0;
	y = x * x;
	y += x;
	y += 1;
	
	return y;
}

void trapezoidal_area(double* area, double a, double b,double n){
	
	double value=0;
	double step=0;

	step = (b-a) / n;
	value = step/2;
	*area += parable(a);
	*area += parable(b);
	a += step;
	while(a<b){
		*area += 2 * parable(a);
		 a += step;
	}
	*area *= value;
	
}

int main(int argc, char* argv[]){
	
	double a=0;
	double area=0;
	double n=0;
	double b=0;
	
	if( argc >= 2 ){
	    if(sscanf(argv[1], "%lf", &a) != 1 ){
		    return (void) fprintf(stderr, "Invalid 'a' value %s\n", argv[1]), 1;
	    }
	    if(sscanf(argv[2], "%lf", &b) != 1){
		    return (void) fprintf(stderr, "Invalid 'b' value %s\n", argv[2]), 1;
	    }
	    if(sscanf(argv[3], "%lf", &n) != 1){
		    return (void) fprintf(stderr, "Invalid 'n' value %s\n", argv[3]), 1;
	    }
    }
	
	
	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	
	trapezoidal_area(&area, a, b, n);
	
	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);
	
	double seconds= finish_time.tv_sec - start_time.tv_sec + 
	(finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
	
	printf("The aproximated area of the parable between %.2f and %.2f is %lf square units and was calculated in %lf seconds\n", a,b,area,seconds);
	
}
