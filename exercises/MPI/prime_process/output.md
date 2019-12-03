[B72097@arenal prime_hybrid_int]$ mpiexec -n 6 -f hosts.txt ./prime_hybrid_int 1 90000000
		5216955 primes found in range [1, 90000000[ in 11.437415s with 6 processes and 48 threads.
	
[B72097@arenal prime_process]$ mpiexec -n 6 -f hosts.txt ./prime_process 1 90000000
		5216955 primes found in range [1, 90000000[ in 40.488431s with 6 processes.


Tiempo menor de prime_hybrid_int: 11.437415s

Tiempo menor de prime_process: 40.488431s

R/ Con prime_hybrid_int se obtienen mejores resultados





