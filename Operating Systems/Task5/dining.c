//[EOPSY.A] Laboratory task 5
//The Dining Philosophers
//Kosmas Tsiakas

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <sys/wait.h>
#include "dining.h"

#define N 5 //define the number of philosophers
#define MAX_DELAY 300000; //define the maximum amount of time that a meal can last
#define MAX_MEALS 1

// Semaphores ids
int forks; //ID for the semaphores array
int start_dinner; //The dinner starts when semaphore reaches 0

//the left fork id is given by the philosopher's number and the right fork id is given by the (num+1)%5

int main(){
    int i, status;
    pid_t philosophers[N];
    printf("Welcome to the dinner philosophers!\n");

    //only for parent process
    //allocate the forks.
    //semaphores are initially equal to 1 and we have 5 forks in an array

    for (i=0; i<N; i++){
        semctl(forks, i, SETVAL, 1);
    }
    start_dinner = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    //the parameters are: key_t key, int nsems, int semflg
    //semflg IPC_CREAT | IPC EXCL
    //IPC_EXCL is used along with IPC_CREAT to guarantee that no existing semaphore set is opened for access

    //prevent children from starting to eat
    semctl(start_dinner, 0, SETVAL, 5);

    //generate the children processes
    for (i=0; i<N; i++){
        int pid = fork();
        if (pid == 0){ //children have pid == 0
            int ret = philosopher(i); //child acts as a philosopher
            exit(ret); //and then exits
        }
        else{ //the parent has pid > 0
            philosophers[i] = pid; //and tracks the children
        }
    }

    //parent waits for children to finish
    for (i=0; i<N; i++){
        waitpid(philosophers[i], &status, 0); //wait for children process to change state
    }

    //cleaning - removing the semaphores
    semctl(forks, 0, IPC_RMID, 0);
    semctl(start_dinner, 0, IPC_RMID, 0);

    return 0;
}

struct sembuf ops; //struct used for semaphores operations
//we didn't define it inside the philosopher() function, because we need access and also from other functions

//each philosophers does the following actions in the row
int philosopher( int num ){
    int i, meals;

    ops.sem_flg = 0; //operation flag set to 0

    //starting the dinner
    ops.sem_op = -1;
    ops.sem_num = 0;
    semop(start_dinner, &ops, 1);
    printf("Philosopher %d is at the dinner\n", num);

    //Waiting for everyone to be ready
    ops.sem_op = 0;
    ops.sem_num = num;
    semop(start_dinner, &ops, 1);

    while(meals<MAX_MEALS){
        //in order to avoid deadlock, we must change the order asking for forks, at least for the last philosopher
        think(num);
        grab_forks(num);
        eat(num);
        put_away_forks(num);
        meals++;
    }
    printf("Goodbye from philosopher #%d\n", num);
    exit(num);
}

void think( int num ){
    int sleep_time = rand() % MAX_DELAY ;
    printf("Philosopher %d is thinking right now\n", num);
    usleep(sleep_time); //sleep for some microseconds
    printf("Philosopher %d is done thinking right now\n", num);
}

void eat( int num ){
    int sleep_time = rand() % MAX_DELAY;
    printf("Philosopher %d is eating right now\n", num);
    usleep(sleep_time);
    printf("Philosopher %d is done eating right now\n", num);
}

void grab_forks( int left_fork_id ){
    int right_fork_id = (left_fork_id + 1) % N;

    if (left_fork_id == N-1){ //if is the last philosopher, ask firstly for the right fork to avoid deadlock
        //get first fork
        ops.sem_op = -1;
        ops.sem_flg = right_fork_id;
        semop(forks, &ops, 1);

        //get second fork
        ops.sem_op = -1;
        ops.sem_num = left_fork_id;
        semop(forks, &ops, 1);
    }
    else{ //the rest of the philosophers, proceed normally
        //get first fork
        ops.sem_op = -1;
        ops.sem_flg = left_fork_id;
        semop(forks, &ops, 1);

        //get second fork
        ops.sem_op = -1;
        ops.sem_num = right_fork_id;
        semop(forks, &ops, 1);
    }
}

void put_away_forks( int left_fork_id ){
    int right_fork_id = (left_fork_id + 1) % N;
    
    //release first fork
    ops.sem_op = +1;
    ops.sem_num = left_fork_id;
    semop(forks, &ops, 1);

    //release second fork
    ops.sem_op = +1;
    ops.sem_num = right_fork_id;
    semop(forks, &ops, 1);
}
