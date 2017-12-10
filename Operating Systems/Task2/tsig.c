//Kosmas Tsiakas
//EOPSY laboratory task 2
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#define NUM_CHILD 5

void sighandler(int signum){
	printf("Caught signal %d, coming out...\n", signum);
	exit(1);
}

int main( int argc, char *argv[] ){

	int i;

	pid_t pid[NUM_CHILD];

	printf("parent[%d] \n", getpid());

	for (i=0; i<NUM_CHILD; i++){
		pid[i] = fork(); //create each child
		if (pid[i] ==0){
			printf("child[%d] - parent's id: %d.\n", getpid(), getppid());

			sleep(10);
			//break;
			exit(0);
		}
		else if (pid[i] == -1){
			//check whether the processes have been created properly
			fprintf(stderr, "fork() failed! error: %d \n", errno); //print error message
			signal(SIGTERM, sighandler);//send SIGTERM signal
			//exit(1);
		}
		sleep(1); //delay 1 second
	}

	printf("parent[%d] All child processes have been created!\n", getpid());

	//wait until no more processes need to be synchronized with the parent
	int terminated = 0;
	for (i = 0; i<NUM_CHILD; i++) {
		int status;
    if ( waitpid(pid[i], &status, 0) == -1 ) {
        perror("waitpid failed");
        return EXIT_FAILURE;
    }
		//get the exit codes
    if ( WIFEXITED(status) ) {
        const int es = WEXITSTATUS(status);
				terminated++;
        printf("parent[%d]: exit status of child[%d] was %d\n",getpid(), pid[i], es);
    }
	}
	printf("Number of terminated processes: %d \n",terminated);

	printf("Execution completed\n");

	return 0;
}
