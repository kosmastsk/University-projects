#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "utils.h"
#include <cilk/cilk.h>
#include "cilk/cilk_api.h"

#define DIM 3

void data_rearrangement(float *Y, float *X, 
			unsigned int *permutation_vector, 
			int N){

  __cilkrts_init();
  __cilkrts_end_cilk();
  __cilkrts_set_param("nworkers", nthreads);

      cilk_for (int i=0; i<N; i++){
        memcpy(&Y[i*DIM], &X[permutation_vector[i]*DIM], DIM*sizeof(float));
  }

}
