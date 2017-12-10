#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <pthread.h>
#include "utils.h"

#define MAXBINS 8

struct bin{
    unsigned long int *morton_codes;
    int shift;
    int *bin_size;
    int start_point;
    int end_point;
    pthread_spinlock_t *spinlock;
};

void *parallel_bins(void* arg){
  struct bin *my_data = (struct bin *) arg;
  int sft = my_data->shift;
  int localBins[MAXBINS] = {0};
  unsigned long int *morton_codes = my_data->morton_codes;

  for ( int k = my_data->start_point; k < my_data->end_point; k++){
    unsigned int ii = (morton_codes[k] >>sft) & 0x07;
    localBins[ii]++;
  }

  //avoid racing conditions
  pthread_spin_lock(my_data->spinlock);

  for (int k=0;k<MAXBINS;k++){
    (my_data->bin_size)[k] += localBins[k];
  }

  pthread_spin_unlock(my_data->spinlock);

}

struct thread_data{
    unsigned long int *morton_codes;
    unsigned long int *sorted_morton_codes;
    unsigned int *permutation_vector;
    unsigned int *index;
    unsigned int *level_record;
    int N;
    int population_threshold;
    int sft;
    int lv;
};

//pthread_mutex_t mutex_mortons;

void *threaded_truncated_radix_sort(void *threadarg){
  //pthread_mutex_lock (&mutex_mortons);
  struct thread_data *my_data;
  my_data = (struct thread_data *) threadarg;
  truncated_radix_sort(my_data->morton_codes, my_data->sorted_morton_codes, my_data->permutation_vector, my_data->index, my_data->level_record, my_data->N, my_data->population_threshold, my_data->sft, my_data->lv);
  //pthread_mutex_unlock (&mutex_mortons);

}

inline void swap_long(unsigned long int **x, unsigned long int **y){

  unsigned long int *tmp;
  tmp = x[0];
  x[0] = y[0];
  y[0] = tmp;

}

inline void swap(unsigned int **x, unsigned int **y){

  unsigned int *tmp;
  tmp = x[0];
  x[0] = y[0];
  y[0] = tmp;

}

void truncated_radix_sort(unsigned long int *morton_codes,
                          unsigned long int *sorted_morton_codes,
                          unsigned int *permutation_vector,
                          unsigned int *index,
                          unsigned int *level_record,
                          int N,
                          int population_threshold,
                          int sft, int lv) {

    //int rc;
    //void *status;

    //pthread_t threads[NUM_THREADS];
    //pthread_mutex_init(&mutex_mortons, NULL);

    //struct thread_data RecData[NUM_THREADS];
    //RecData->morton_codes = morton_codes;
    //RecData->sorted_morton_codes = sorted_morton_codes;
    //RecData->permutation_vector = permutation_vector;
    //RecData->index = index;
    //RecData->level_record = level_record;
    //RecData->N = N;
    //RecData->population_threshold = population_threshold;
    //RecData->sft = sft;
    //RecData->lv = lv;

    //pthread_attr_t attribute;
    //pthread_attr_init(&attribute);
    //pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

    int BinSizes[MAXBINS] = {0};
    int BinCursor[MAXBINS] = {0};
    unsigned int *tmp_ptr;
    unsigned long int *tmp_code;

    if (N <= 0) {

      return;

    } else if (N <= population_threshold || sft < 0) { // Base case. The node is a leaf

      level_record[0] = lv; // record the level of the node
      memcpy(permutation_vector, index, N * sizeof(unsigned int)); // Copy the permutation vector
      memcpy(sorted_morton_codes, morton_codes, N * sizeof(unsigned long int)); // Copy the Morton codes

      return;
    } else {

      level_record[0] = lv;
      if (lv<1){

        pthread_t threads[NUM_THREADS];
        //Declare the attribute
        pthread_attr_t attribute;
        void *status;

        //Initialize a spinlock
        pthread_spinlock_t lock;

        pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);

        //make the threads joinable
        pthread_attr_init(&attribute);
        pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

        int chunk = N / NUM_THREADS;

        struct bin binData[NUM_THREADS];

        //set the data to pass in every thread
        for ( long i = 0; i<NUM_THREADS; i++){
          (binData+i)->morton_codes = morton_codes;
          (binData+i)->shift = sft;
          (binData+i)->bin_size = &BinSizes[0];
          (binData+i)->spinlock = &lock;
          (binData+i)->start_point = i*chunk;
          (binData+i)->end_point = (i+1)*chunk;

          pthread_create(&threads[i], &attribute, parallel_bins, (void*) &binData[i] );
        }

        pthread_attr_destroy(&attribute);
        for (long i=0 ;i<NUM_THREADS; i++){
          pthread_join( threads[i], &status);
        }

      // Find which child each point belongs to
      //for (int j = 0; j < N; j++) {
        //unsigned int ii = (morton_codes[j] >> sft) & 0x07;
        //BinSizes[ii]++;
      //}

      // scan prefix (must change this code) //hope it's false
      int offset = 0;
      for (int i = 0; i < MAXBINS; i++) {
        int ss = BinSizes[i];
        BinCursor[i] = offset;
        offset += ss;
        BinSizes[i] = offset;
      }

      for (int j = 0; j < N; j++) {
        unsigned int ii = (morton_codes[j] >> sft) & 0x07;
        permutation_vector[BinCursor[ii]] = index[j];
        sorted_morton_codes[BinCursor[ii]] = morton_codes[j];
        BinCursor[ii]++;
      }
      //swap the index pointers
      swap(&index, &permutation_vector);

      //swap the code pointers
      swap_long(&morton_codes, &sorted_morton_codes);

      //Initialize a table with threads for the threads containing the recursive calls
      pthread_t rec_threads[MAXBINS];

      pthread_attr_t attributeRec;

      pthread_attr_init(&attributeRec);
      pthread_attr_setdetachstate(&attributeRec, PTHREAD_CREATE_JOINABLE);
      
      struct thread_data RecData[MAXBINS];

      long i;
      /* Call the function recursively to split the lower levels */
      for (i = 0; i < MAXBINS; i++) {
        int offset = (i > 0) ? BinSizes[i - 1] : 0;
        int size = BinSizes[i] - offset;
        RecData[i].morton_codes = &morton_codes[offset];
        RecData[i].sorted_morton_codes = &sorted_morton_codes[offset];
        RecData[i].permutation_vector = &permutation_vector[offset];
        RecData[i].index = &index[offset];
        RecData[i].level_record = &level_record[offset];
        RecData[i].N = size;
        RecData[i].population_threshold = population_threshold;
        RecData[i].sft = sft - 3;
        RecData[i].lv = lv + 1;
        int rc = pthread_create(&rec_threads[i], &attributeRec, threaded_truncated_radix_sort, (void *) &RecData[i]);
        if (rc) {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
        }
      }

      pthread_attr_destroy(&attributeRec);

      for (long i = 0; i < MAXBINS; i++) {
        int rc = pthread_join(rec_threads[i], &status);
        if (rc) {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
        }
      }
    }

  else{
        for (int j=0; j<N; j++){
          unsigned int ii = (morton_codes[j]>>sft) & 0x07;
          BinSizes[ii]++;
        }

        //scan prefix (must change this code)
        int offset = 0;
        for (int i=0; i<MAXBINS; i++){
          int ss = BinSizes[i];
          BinCursor[i] = offset;
          offset += ss;
          BinSizes[i] = offset;
        }

        for(int j=0; j<N; j++){
          unsigned int ii = (morton_codes[j]>>sft) & 0x07;
          permutation_vector[BinCursor[ii]] = index[j];
          sorted_morton_codes[BinCursor[ii]] = morton_codes[j];
          BinCursor[ii]++;
        }

        //swap the index pointers
        swap(&index, &permutation_vector);

        //swap the code pointers
        swap_long(&morton_codes, &sorted_morton_codes);

        /*Call the function recursively to split the loewr levels */
        for (int i=0; i<MAXBINS; i++){
          int offset = (i>0) ? BinSizes[i-1] : 0;
          int size = BinSizes[i] - offset;

          truncated_radix_sort(&morton_codes[offset],
                               &sorted_morton_codes[offset],
                               &permutation_vector[offset],
                               &index[offset], &level_record[offset],
                               size,
                               population_threshold,
                               sft-3, lv+1);
        }

      }
    }
}

