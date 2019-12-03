#include <stdio.h>

int main()
{
	const char* filename = "hello.c"; 
	FILE* file = fopen(filename,"r"); //retorna un puntero a esta pos del registro en mem
	if( !file )
		return (void)fprintf(stderr, "error: could not open %s\n", filename), 1; 
		//Este 1 es como un EXIT_FAILURE
		
	int ch = 0;
	while( (ch=fgetc(file)) != EOF ) //end of file
			fputc(ch, stdout);	
	fclose(file);		
	return 0;
}
