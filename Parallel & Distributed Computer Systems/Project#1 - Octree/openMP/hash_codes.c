#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "float.h"
#include "omp.h"

#define DIM 3

inline unsigned int compute_code(float x, float low, float step){

  return floor((x - low) / step);

}


/* Function that does the quantization */
void quantize(unsigned int *codes, float *X, float *low, float step, int N) {
    int i = 0;
    int j = 0;
    int pos = 0;
    int thr=0;
    #pragma omp parallel private(i, j, pos)
    {
    #pragma omp for schedule(guided)
    for (i = 0; i < N; i++) {
        for (j = 0; j < DIM; j++) {
            pos = i * DIM + j;
            codes[pos] = compute_code(X[pos], low[j], step);
        }
    }
}

}

float max_range(float *x){
    int i=0;
  float max = -FLT_MAX;
  for(i=0; i<DIM; i++){
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
    int i=0;
    for(i=0; i<DIM; i++){
    range[i] = fabs(max[i] - min[i]); // The range of the data
    range[i] += 0.01*range[i]; // Add somthing small to avoid having points exactly at the boundaries
  }

  qstep = max_range(range) / nbins; // The quantization step 
  
  quantize(codes, X, min, qstep, N); // Function that does the quantization

}



