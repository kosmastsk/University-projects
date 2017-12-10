#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "float.h"
#include "pthread.h"
#include "string.h"
#include "utils.h"

#define DIM 3


inline unsigned int compute_code(float x, float low, float step){

  return floor((x - low) / step);

}


typedef struct{
    unsigned int *codes;
    float *X;
    float *low;
    float step;
    int N;
}parallel_hash_data;

void* parallel_quantize(void* arg){
  parallel_hash_data *my_data = (parallel_hash_data*) arg;
  my_data = malloc((my_data->N)*sizeof(parallel_hash_data));

  for(int i=0; i<my_data->N; i++){
    for(int j=0; j<DIM; j++){
      my_data->codes[i*DIM + j] = compute_code(my_data->X[i*DIM + j], my_data->low[j], my_data->step);
    }
  }

  free(my_data);

}


/* Function that does the quantization */
void quantize(unsigned int *codes, float *X, float *low, float step, int N){
  // Initiliaze the static array containing the identities of threads/
  pthread_t threads[NUM_THREADS];
  // Declare an attribute for the above threads.
  pthread_attr_t tattr;
  void* status;


  // Initiliaze the attribute so that the threads we create are
  // joinable.
  pthread_attr_init(&tattr);
  pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);

  parallel_hash_data *hash_data;
  hash_data = malloc((NUM_THREADS*sizeof(parallel_hash_data)));
  // Split the data into even chunks for every thread.
  int chunk = N / NUM_THREADS ;

  for (long i = 0 ; i<NUM_THREADS ; ++i) {
    (hash_data+i)->N = chunk;
    long start = i*chunk;
    (hash_data + i)->codes = malloc(chunk * sizeof(unsigned int));

    memcpy((hash_data + i)->codes, codes + start, chunk * sizeof(unsigned int));

    pthread_create(&threads[i],&tattr,parallel_quantize,(hash_data+i));
  }

  pthread_attr_destroy(&tattr);
  for ( long i = 0; i < NUM_THREADS; ++i)
  {
    pthread_join(threads[i], &status);
  }
  free(hash_data);




}


///* Function that does the quantization */
//void quantize(unsigned int *codes, float *X, float *low, float step, int N){
//
//  for(int i=0; i<N; i++){
//    for(int j=0; j<DIM; j++){
//      codes[i*DIM + j] = compute_code(X[i*DIM + j], low[j], step);
//    }
//  }
//}



float max_range(float *x){

  float max = -FLT_MAX;
  for(int i=0; i<DIM; i++){
    if(max<x[i]){
      max = x[i];
    }
  }

  return max;

}

void compute_hash_codes(unsigned int *codes, float *X, int N,
                        int nbins, float *min,
                        float *max){

  float range[DIM];
  float qstep;

  for(int i=0; i<DIM; i++){
    range[i] = fabs(max[i] - min[i]); // The range of the data
    range[i] += 0.01*range[i]; // Add somthing small to avoid having points exactly at the boundaries
  }

  qstep = max_range(range) / nbins; // The quantization step

  quantize(codes, X, min, qstep, N); // Function that does the quantization

}


