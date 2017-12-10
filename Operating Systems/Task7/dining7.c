//`[EOPSY.A] Laboratory task #7
//Kosmas Tsiakas
//The Dining Philosophers problem implemented with threads and mutexes from pthread library

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM 5 //define the number of philosophers

// States of a philosopher
#define THINKING 0
#define HUNGRY 1
#define EATING 2
int food = 15; //The total amount of food on the table. //each philosopher will eat food/NUM times

int monitor = HUNGRY;
int state[NUM]; //Save the state of the philosophers during the simulation

//The mutex for monitor variable and the condition resource_ready
pthread_mutex_t monitor_mutex;
pthread_cond_t resource_ready;
//The philosopher thread identifiers
pthread_t philo[NUM]; //one thread for each philosopher, save them in an array

void *philosopher (void *num);
void grab_forks (int, int, int);
void test (int);
void put_away_forks (int, int, int);

int main (int argc, char **argv)
{
	int i;

	pthread_mutex_init (&monitor_mutex, NULL); //initialize mutexes with 0
	pthread_cond_init (&resource_ready, NULL);

	//Create a thread for each philosopher
	for (i = 0; i < NUM; i++){
		//initialize each thread by calling the routine philosopher()
		pthread_create (&philo[i], NULL, philosopher, (void *)i);
	}

	//Wait for the threads to exit
	for (i = 0; i < NUM; i++){
		pthread_join (philo[i], NULL);
	}

	pthread_exit(NULL);

	return 0;
}

void *philosopher (void *num){
	int id;
	int i, left_fork, right_fork, f;

	id = (int)num; //This is the philosopher's id

	state[id] = THINKING;
	printf ("Philosopher %d is thinking.\n", id);
	sleep(1);

	//Identify the philosopher's right and left chopstick
	right_fork = (id + 1) % NUM ;
	left_fork = (id + (NUM - 1)) % NUM;

	//While there is still food on the table, the loop goes on
	while (food > 0)
	{
		//Get the chopstick
		grab_forks (id, left_fork, right_fork);
		sleep(1);
	}
	return (NULL);
}

void grab_forks (int phil, int left_fork, int right_fork){
	//Lock monitor variable
	pthread_mutex_lock (&monitor_mutex);
	state[phil] = monitor; //monitor is initialized with HUNGRY
	test(phil);
	if (state[phil] == EATING) //
	{
		pthread_cond_wait (&resource_ready, &monitor_mutex);
		printf ("Philosopher %d is now eating\n", phil);
		food--; //1 food is consumed
		if (food == 0)
		{
			printf ("Food is over. Hope you enjoyed your dinner!\n");
			pthread_mutex_destroy(&monitor_mutex);
			pthread_cond_destroy(&resource_ready);
			exit(1);
		}
		put_away_forks (phil, left_fork, right_fork);
		sleep(1);
	}
	//Unlock monitor variable
	pthread_mutex_unlock (&monitor_mutex);
}

void test (int phil){
	//If the philosopher is hungry and the nearest 2 are not eating, he then eats.
	if ((state[(phil + NUM - 1) % NUM] != EATING) && (state[phil] == HUNGRY)){
		state[phil] = EATING;
		pthread_cond_signal (&resource_ready); //unblocks at least one of the thread that are blocked on this variable
	}
}
void put_away_forks (int phil, int c1, int c2)
{
	state[phil] = THINKING;
	//Check the 2 nearest philosophers to see if they are eating
	test ((phil + NUM - 1) % NUM);
	test ((phil + 1) % NUM);
}