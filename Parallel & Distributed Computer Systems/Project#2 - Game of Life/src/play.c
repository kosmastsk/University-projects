#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>
#include "omp.h"
#include "sys/time.h"


#include "../include/game-of-life.h"

void play (int *board, int *newboard, int N1, int N2, int rank, int numtasks) {
  /*
    (copied this from some web page, hence the English spellings...)

    1.STASIS : If, for a given cell, the number of on neighbours is 
    exactly two, the cell maintains its status quo into the next 
    generation. If the cell is on, it stays on, if it is off, it stays off.

    2.GROWTH : If the number of on neighbours is exactly three, the cell 
    will be on in the next generation. This is regardless of the cell's
    current state.

    3.DEATH : If the number of on neighbours is 0, 1, 4-8, the cell will 
    be off in the next generation.
  */
  /* PERSONAL NOTE FOR PARALLELIZING */
  /* Since the counting of neighbors and the decision of what to do with any cell into the next generation is
   * independent of any other cell, we can parallelize the cell computation within a generation
  */
  int i, j, a;
  omp_set_num_threads(8); //set the number of threads for openMP parallelization

  char my_cpu_name[BUFSIZ];
  int my_name_length;
  MPI_Get_processor_name(my_cpu_name, &my_name_length);
  printf("Hello from processor %s and rank %d\n", my_cpu_name, rank);

  if (numtasks == 1){
    /* for each cell, apply the rules of Life */
    #pragma omp parallel for default(none) shared(board, N2, N1, newboard) private(i, a, j) collapse(2)
    for (i = 0; i < N1; i++) {
      for (j = 0; j < N2; j++) {
        a = adjacent_to(board, i, j, N1, N2);
        if (a == 2) NewBoard(i, j) = Board(i, j);
        if (a == 3) NewBoard(i, j) = 1;
        if (a < 2) NewBoard(i, j) = 0;
        if (a > 3) NewBoard(i, j) = 0;
      }
    }
    /* copy the new board back into the old board */
    #pragma omp parallel for default(none) shared(board, newboard, N2, N1) private(i, j) collapse(2)
    for (i=0; i < N1; i++) {
      for (j = 0; j < N2; j++) {
        Board(i, j) = NewBoard(i, j);
      }
    }

  }
  else {

    int tag1 = 1;
    int tag2 = 2;

    // set the previous and next rank to communicate between them
    int prev = rank - 1;
    int next = rank + 1;
    // take care of cyclic boundary conditions
    if (rank == 0) prev = numtasks - 1;
    if (rank == (numtasks - 1)) next = 0;

    MPI_Request reqs[4];
    MPI_Status stats[4];
    int flags[4];

    //send and receive buffers
    int out1[N2], out2[N2], in1[N2], in2[N2];

    #pragma omp parallel for default(none) shared(board, N1, N2, out1, out2) private(j)
    for (j = 0; j < N2; j++) {
      out1[j] = Board(0, j);
      out2[j] = Board(N1 - 1, j);
    }

    //Send data to the previous part of board
    MPI_Isend(out1, N2, MPI_INT, prev, tag1, MPI_COMM_WORLD, &reqs[0]);

    //Send data to the next part of board
    MPI_Isend(out2, N2, MPI_INT, next, tag2, MPI_COMM_WORLD, &reqs[1]);

    //Receive data from the previous part of board, since send has been completed
    MPI_Irecv(in2, N2, MPI_INT, prev, tag2, MPI_COMM_WORLD, &reqs[2]);

    //Receive data from the next part of board
    MPI_Irecv(in1, N2, MPI_INT, next, tag1, MPI_COMM_WORLD, &reqs[3]);

    //do some work until send is done

    /* for each cell, apply the rules of Life */
    #pragma omp parallel for default(none) shared(board, N2, N1, newboard) private(i, j, a) collapse(2)
    //do not include the first and last line in play, these are used just to ensure cyclic boundary conditions
    for (i = 2; i < N1-2; i++){
      for (j = 0; j < N2; j++){

        a = adjacent_to(board, i, j, N1, N2);
        if (a == 2) NewBoard(i, j) = Board(i, j);
        if (a == 3) NewBoard(i, j) = 1;
        if (a < 2) NewBoard(i, j) = 0;
        if (a > 3) NewBoard(i, j) = 0;

      }
    }

    //wait until sending is complete and then start receiving
    MPI_Waitall(4, reqs, stats);

    #pragma omp parallel for default(none) shared(board, newboard, N1, N2, in1, in2) private(j, a)
    for (j = 0; j < N2; j++){

      //copy what I received in my board
      Board(N1 - 1, j) = in1[j];
      Board(0, j) = in2[j];

      //calculate neighbors for them
      a = adjacent_to(board, 1, j, N1, N2);
      if (a == 2) NewBoard(1, j) = Board(1, j);
      if (a == 3) NewBoard(1, j) = 1;
      if (a < 2) NewBoard(1, j) = 0;
      if (a > 3) NewBoard(1, j) = 0;

      a = adjacent_to(board, N1-1, j, N1, N2);
      if (a == 2) NewBoard(N1-1, j) = Board(N1-1, j);
      if (a == 3) NewBoard(N1-1, j) = 1;
      if (a < 2) NewBoard(N1-1, j) = 0;
      if (a > 3) NewBoard(N1-1, j) = 0;

    }

    /* copy the new board back into the old board */
    #pragma omp parallel for default(none) shared(board, newboard, N2, N1) private(i, j, a) collapse(2)
    for (i=1; i < N1-1; i++) {
      for (j = 0; j < N2; j++){
        Board(i, j) = NewBoard(i, j);
      }
    }

  }

} // end of play function
