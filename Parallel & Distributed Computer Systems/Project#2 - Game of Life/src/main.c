/*
 * Game of Life implementation based on
 * http://www.cs.utexas.edu/users/djimenez/utsa/cs1713-3/c/life.txt
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"
#include <omp.h>
#include "sys/time.h"

#include "../include/game-of-life.h"

// to set the size of the board later

int main (int argc, char *argv[]) {
  int *board, *newboard, i;

  if (argc != 6) { // Check if the command line arguments are correct
    printf("Usage: %s Ν1 Ν2 thres disp iter thr t\n"
                   "where\n"
                   "  N1    : number of board's lines\n"
                   "  N2    : number of board's columns\n"
                   "  thres : propability of alive cell\n"
                   "  disp  : {1: display output, 0: hide output}\n"
                   "  iter  : number of generations\n", argv[0]);
    return (1);
  }

  // Input command line arguments
  int N1 = atoi(argv[1]);                   // Rows number
  int N2 = atoi(argv[2]);                   // Column number
  double thres = atof(argv[3]);             // Probability of life cell
  int disp = atoi(argv[4]);                 // Display output?
  int t = atoi(argv[5]);                    // Three generations on each execution

  printf("Size %dx%d with propability: %0.1f%%\n", N1, N2, thres * 100);

  int rc, numtasks, rank; //use this variable to check status of MPI functions

  // Time counting variables
  struct timeval startwtime, endwtime;

  rc = MPI_Init(&argc, &argv);
  if (rc != MPI_SUCCESS) {
    printf("Error starting MPI program. Terminating.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  //set a barrier to start counting at the same condition for every process
  MPI_Barrier(MPI_COMM_WORLD);
  //start time!
  gettimeofday (&startwtime, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Hello from rank: %i of %i numtasks \n", rank, numtasks);

  board = NULL;
  newboard = NULL;
  //each process creates a smaller board depending on the number of tasks
  //so we change N1 to cut the board in lines
  //in case we meed communication add new lines
  if (numtasks != 1){
    N1 = N1/numtasks + 2; //create a new line on the first line and on the last line
  }
  //printf("N1 = %d\n", N1);
  board = (int *) malloc(N1 * N2 * sizeof(int));
  if (board == NULL) {
    printf("\nERROR: Memory allocation did not complete successfully!\n");
    return (1);
  }

  /* second pointer for updated result */
  newboard = (int *) malloc(N1 * N2 * sizeof(int));

  if (newboard == NULL) {
    printf("\nERROR: Memory allocation did not complete successfully!\n");
    return (1);
  }

  /*functions contained in init.c */
  //initialize_board(board, N1, N2); //this function is useless as we have generate below
  //printf("Board initialized\n");
  generate_table(board, N1, N2, thres, numtasks);
  printf("Board generated\n");

  /* play game of life t times */
  /* t generations */

  // each rank gets the code and executes it independently
  for (i = 0; i < t; i++) { //can't parallelize this loop, it is a time series
    if (disp) {
      display_table(board, N1, N2);//display the new condition of the board
    }
    play(board, newboard, N1, N2, rank, numtasks);//changed this. if disp was 0, play was never executed
  }

  //print the last condition of the board
  if (disp) {
    print(board, N1, N2);
  }
  printf("Game finished after %d generations.\n\n", t);
  //set a barrier to stop timer after all processes have reached the same point
  MPI_Barrier(MPI_COMM_WORLD);
  //stop timer!
  gettimeofday (&endwtime, NULL);
  double total_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
  //free space
  free(board);
  free(newboard);
  MPI_Finalize();

  printf("Total execution time for all processes : %fs\n", total_time);

  return(0);

}

