#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifdef __APPLE__
#include "pthread_barrier.h"
#endif

// Data shared for al threads
typedef struct
{
	// Number of teams given by user, two threads are created for each team
	size_t team_count;
	// Time in milliseconds that threads wait in stage 1: from start to the partner
	unsigned stage_time_1;
	// Time in milliseconds that threads wait in stage 2: from partner to the finish
	unsigned stage_time_2;
	// Counter located at the finish used to know the position each thread reach
	size_t position;
	// A barrier is used at the start of the race to simulate threads departing at the same time
	pthread_barrier_t starting_barrier;
	// An array of semaphores, one for each team, to simulate the batons. Initially, runner 2 of
	// the team will be waiting for the semaphore. When runner 1 reaches runner 2, runner 1 will
	// post to the semaphore
	sem_t* baton_semaphores;
	// A mutex at the finish of the race allows only one runner at time claiming its position
	pthread_mutex_t finish_mutex;
} shared_data_t;

// Private thread data: each thread has its own record
typedef struct
{
	// The number of the thread: range [0, team_count[ identifies runners from start to the middle
	// while range [team_count, 2*team_count[ identify runners from middle to the finish
	size_t thread_id;
	// Pointer to the shared data record for all threads
	shared_data_t* shared_data;
} private_data_t;


int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
void* start_race(void* data);
void* finish_race(void* data);

int main(int argc, char* argv[])
{
	// Fill the shared data with the values from the arguments
	shared_data_t shared_data;
	int error = analyze_arguments(argc, argv, &shared_data);
	if ( error ) return error;

	// Init synchronization controls in shared data
	shared_data.position = 0;
	pthread_barrier_init( &shared_data.starting_barrier, NULL, shared_data.team_count );
	pthread_mutex_init( &shared_data.finish_mutex, NULL );

	// Init the semaphores used as batons, one for each team
	shared_data.baton_semaphores = (sem_t*) calloc( shared_data.team_count, sizeof(sem_t) );
	if ( shared_data.baton_semaphores == NULL )
		return (void)fprintf(stderr, "hello_w: error: could not allocate memory for: %zu semaphores\n", shared_data.team_count), 5;

	// Create records to control each thread, two per team
	const size_t thread_count = 2 * shared_data.team_count;
	pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
	if ( threads == NULL )
		return (void)fprintf(stderr, "hello_w: error: could not allocate memory for: %zu threads\n", thread_count), 6;

	// Create a private record, one for each thread (two per team)
	private_data_t* private_data = (private_data_t*) calloc(thread_count, sizeof(private_data_t));

	// Get a time snapshot to calculate the duration later
	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);

	// Create the threads, two for each team
  #ifdef INVERTED_TEAM_ORDER
	for ( size_t team = shared_data.team_count - 1; team <= shared_data.team_count - 1; --team )
  #else
	for ( size_t team = 0; team < shared_data.team_count; ++team )
  #endif
	{
		// Create the team's thread that starts the race
		private_data[team].thread_id = team;
		private_data[team].shared_data = &shared_data;
		pthread_create(&threads[team], NULL, start_race, private_data + team);

		// Create the partner team's thread that finishes the race
		const size_t partner = team + shared_data.team_count;
		assert(partner < thread_count);
		private_data[partner].thread_id = partner;
		private_data[partner].shared_data = &shared_data;
		pthread_create(&threads[partner], NULL, finish_race, private_data + partner);
	}

	// Wait until the race finishes
	for ( size_t index = 0; index < thread_count; ++index )
		pthread_join(threads[index], NULL);

	// Get the finish time as another time snapshot
	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);

	// Calculate the simulation time
	const double seconds = finish_time.tv_sec - start_time.tv_sec
		+ (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
	printf("Simulation time: %.9lfs\n", seconds);

	// Release synchronization mechanisms
	pthread_mutex_destroy(&shared_data.finish_mutex);
	pthread_barrier_destroy( &shared_data.starting_barrier );

	// Release heap memory
	free(shared_data.baton_semaphores);
	free(private_data);
	free(threads);

	return EXIT_SUCCESS;
}

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data)
{
	// Three paramets are mandatory
	if ( argc != 4 )
		return (void)fprintf(stderr, "usage: relay_race <teams> <stage_time_1> <stage_time_2>\n"), 1;

	// Convert text arguments to the shared integer values
	if ( sscanf(argv[1], "%zu", &shared_data->team_count) != 1 || shared_data->team_count == 0 )
		return (void)fprintf(stderr, "hello_w: error: invalid team count: %s\n", argv[1]), 2;
	if ( sscanf(argv[2], "%u", &shared_data->stage_time_1) != 1 )
		return (void)fprintf(stderr, "hello_w: error: invalid stage time 1: %s\n", argv[2]), 3;
	if ( sscanf(argv[3], "%u", &shared_data->stage_time_2) != 1 )
		return (void)fprintf(stderr, "hello_w: error: invalid stage time 2: %s\n", argv[3]), 4;

	return 0;
}

// Threads that depart from the start, execute this function. They goal is to give the baton
// to their partners as fast as they can
void* start_race(void* data)
{
	// Get pointers to the private and shared data
	private_data_t* private_data = (private_data_t*)data;
	shared_data_t* shared_data = private_data->shared_data;

	// Wait at the starting line
	pthread_barrier_wait( &shared_data->starting_barrier );

	// The race started! Traverse the stage 1. It takes time
	usleep( 1000 * shared_data->stage_time_1 );

	// I reached my partner, give it the baton
	const size_t team_id = private_data->thread_id;
	sem_post( &shared_data->baton_semaphores[team_id] );

	// I finished my race
	return NULL;
}

void* finish_race(void* data)
{
	// Get pointers to the private and shared data
	private_data_t* private_data = (private_data_t*)data;
	shared_data_t* shared_data = private_data->shared_data;

	// I wait until my partner gives me the baton
	size_t team_id = private_data->thread_id - shared_data->team_count;
	assert(team_id < shared_data->team_count);
	sem_wait( &shared_data->baton_semaphores[team_id] );

	// My partner gave me the beaton! Traverse the stage 2. It takes time
	usleep( 1000 * shared_data->stage_time_2 );

	// I arrived to the finish line! grab my position
	pthread_mutex_lock( &shared_data->finish_mutex );
	++shared_data->position;

	// Only first runners win the race
	if ( shared_data->position <= 3 )
	{
		// Report the place that my team reached
		printf( "Place %zu: team %zu\n", shared_data->position, team_id + 1 );
	}

	// Leave the finish to allow other teams to traverse it
	pthread_mutex_unlock( &shared_data->finish_mutex );

	// I finished my race
	return NULL;
}
