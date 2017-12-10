#include "stdio.h"
#include "stdlib.h"
#include <pthread.h>
#include "string.h"
#include "utils.h"


#define DIM 3
//#define THREADS 4

struct mortons{
    int N;
    unsigned long int *mcodes;
    unsigned int *codes;
    long start_point;
    long end_point;
};


inline unsigned long int splitBy3(unsigned int a){
  unsigned long int x = a & 0x1fffff; // we only look at the first 21 bits
  x = (x | x << 32) & 0x1f00000000ffff;  // shift left 32 bits, OR with self, and 00011111000000000000000000000000000000001111111111111111
  x = (x | x << 16) & 0x1f0000ff0000ff;  // shift left 32 bits, OR with self, and 00011111000000000000000011111111000000000000000011111111
  x = (x | x << 8) & 0x100f00f00f00f00f; // shift left 32 bits, OR with self, and 0001000000001111000000001111000000001111000000001111000000000000
  x = (x | x << 4) & 0x10c30c30c30c30c3; // shift left 32 bits, OR with self, and 0001000011000011000011000011000011000011000011000011000100000000
  x = (x | x << 2) & 0x1249249249249249;
  return x;
}

inline unsigned long int mortonEncode_magicbits(unsigned int x, unsigned int y, unsigned int z){
  unsigned long int answer;
  answer = splitBy3(x) | splitBy3(y) << 1 | splitBy3(z) << 2;
  return answer;
}

void* parallel_morton_codes(void* arg){

  struct mortons *my_codes = (struct mortons*) arg;

  for (int i = my_codes->start_point ; i<my_codes->end_point ; ++i){

    my_codes->mcodes[i] = mortonEncode_magicbits(
            my_codes->codes [i*DIM],
            my_codes->codes [i*DIM +1],
            my_codes->codes [i*DIM +2]
    );
  }

  pthread_exit((void*) 0);
}


void morton_encoding(unsigned long int *mcodes, unsigned int *codes, int N, int max_level){

  struct mortons tcodes[NUM_THREADS];
  //gcodes = malloc(NUM_THREADS*sizeof(Codes));
  //gcodes->mcodes=mcodes;

  pthread_t *threads = malloc(NUM_THREADS*sizeof(pthread_t));
  //pthread_t threads[NUM_THREADS];
  pthread_attr_t attribute;
  void* status;
  long i;

  pthread_attr_init(&attribute);
  pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

  int chunk = N / NUM_THREADS;

  for (i = 0 ; i<NUM_THREADS ; ++i) {
    (tcodes+i)->N = chunk;

    long start = i * chunk;
    long end = (i+1)*chunk;
    (tcodes+i)->codes = codes;
    (tcodes+i)->mcodes = mcodes;
    (tcodes+i)->start_point = start;
    (tcodes+i)->end_point = end;

    pthread_create(&threads[i],&attribute,parallel_morton_codes,(void*)&tcodes[i]);
  }

  pthread_attr_destroy(&attribute);
  for ( i = 0; i < NUM_THREADS; ++i)
  {
    pthread_join(threads[i], &status);

  }
  //free(tcodes);
  free(threads);
}
