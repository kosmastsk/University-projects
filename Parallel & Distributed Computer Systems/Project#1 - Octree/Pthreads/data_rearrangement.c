#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "pthread.h"
#include "utils.h"

#define DIM 3

typedef struct{
    float *Y;
    float *X;
    unsigned int *permutation_vector;
    int N;
    int start;
    int end;
}parallel_data;


void* parallel_data_rearrangement(void* arg){

  parallel_data *my_data = (parallel_data*) arg;
  //my_data = malloc((my_data->N)*sizeof(parallel_data));



  for(int i=my_data->start; i<my_data->end; i++){
    memcpy(&my_data->Y[i*DIM], &my_data->X[my_data->permutation_vector[i]*DIM], DIM*sizeof(float));
  }

  //free(my_data);
}

void data_rearrangement(float *Y, float *X,
                        unsigned int *permutation_vector,
                        int N){

  // Initiliaze the static array containing the identities of NUM_THREADS/
  //pthread_t NUM_THREADS[NUM_THREADS];
  pthread_t *THREADS = malloc(NUM_THREADS*sizeof(pthread_t));
  // Declare an attribute for the above NUM_THREADS.
  pthread_attr_t tattr;
  void* status;

  // Initiliaze the attribute so that the NUM_THREADS we create are
  // joinable.
  pthread_attr_init(&tattr);
  pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);

//    parallel_data *data;
//    data = malloc((NUM_THREADS*sizeof(parallel_data)));
  parallel_data data[NUM_THREADS];

  int chunk = N/NUM_THREADS;
  for (long i = 0 ; i<NUM_THREADS ; ++i) {
    (data+i)->N = chunk;
    int start = i*chunk;
    int end = (i+1)*chunk;
//        (data + i)->X = malloc(chunk * sizeof(float));
//        (data + i)->Y = malloc(chunk * sizeof(float));
//        (data + i)->permutation_vector = malloc(chunk * sizeof(unsigned int));

    (data + i)->X = X;
    (data + i)->Y = Y;
    (data + i)->permutation_vector = permutation_vector;
    (data + i)->start = start;
    (data + i)->end = end;


//        memcpy((data + i)->X, X + start, chunk * sizeof(unsigned int));
//        memcpy((data + i)->Y, Y + start, chunk * sizeof(unsigned int));
//        memcpy((data + i)->permutation_vector, permutation_vector + start, chunk * sizeof(unsigned int));


    pthread_create(&THREADS[i],&tattr,parallel_data_rearrangement,(void*) &(data[i]));
  }

  pthread_attr_destroy(&tattr);
  for ( long i = 0; i < NUM_THREADS; ++i)
  {
    pthread_join(THREADS[i], &status);
  }
  //free(data);





}