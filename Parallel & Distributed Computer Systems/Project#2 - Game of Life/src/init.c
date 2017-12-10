#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

#include "../include/game-of-life.h"

//THIS FUNCTION IS NEVER USED.
/* set everything to zero */
void initialize_board (int *board, int N1, int N2) {
  int i, j;
  for (i = 0; i < N1; i++) {
    for (j = 0; j < N2; j++) {
      Board(i, j) = 0;
    }
  }
}

/* generate random table */

  void generate_table(int *board, int N1, int N2, double threshold, int numtasks) {
    int i, j;
    //int counter = 0;
  omp_set_num_threads(8); //set the number of threads for openMP parallelization

  srand(time(NULL));
  // use rand_r() because it is thread safe and returns better results
  unsigned int myseed;
  #pragma omp parallel for default(none) shared(board, N1, N2, threshold, numtasks) private(i ,j, myseed) collapse(2)
    for (i = 0; i < N1; i++){ //first fill in the rows and then the columns
      for (j = 0; j < N2; j++){
        if ((i==0 || i==N1-1) && (numtasks!=1) ) {
          Board(i, j) = 0;
        }
        else{
          myseed = omp_get_thread_num();
          Board(i,j) = ((float)rand_r(&myseed) / (float)RAND_MAX) < threshold;
        }
      }
    }

  }

